
#include "JexFluidSystem.h"

namespace Jex {

	JexFluidSystem::JexFluidSystem() {

		unit_scale = 0.004;
		viscosity = 1.0;
		rest_density = 1000.0;
		particle_mass = 0.0004;
		gas_constant = 1.0;
		smooth_radius = 0.01;

		boundary_stiffness = 10000.0;
		boundary_dampening = 256.0;
		speed_limit = 200.0;

		poly6_kernel = 315.0 / (64.0 * PI * pow(smooth_radius, 9));
		spiky_kernel = -45.0 / (PI * pow(smooth_radius, 6));
		visco_kernel = 45.0 / (PI * pow(smooth_radius, 6));
	}

	JexFluidSystem::~JexFluidSystem() {

	}

	void JexFluidSystem::AddFluidVolume(const Box & fluid_box, float spacing) {

		float cx = 0.5 * (fluid_box.MaxVertex().X() + fluid_box.MinVertex().X());
		float cy = 0.5 * (fluid_box.MaxVertex().Y() + fluid_box.MinVertex().Y());
		float cz = 0.5 * (fluid_box.MaxVertex().Z() + fluid_box.MinVertex().Z());
		
		for (float z = fluid_box.MaxVertex().Z(); z >= fluid_box.MinVertex().Z(); z -= spacing) {
			for (float y = fluid_box.MinVertex().Y(); y <= fluid_box.MaxVertex().Y(); y += spacing) {
				for (float x = fluid_box.MinVertex().X(); x <= fluid_box.MaxVertex().X(); x += spacing) {
					//printf("%f %f %f \n", x, y, z);
					Particle * ptc = ptc_buffer.AddParticleReuse();
					ptc->position.set(x, y, z);
					//ptc->position.print_info();
				}
			}
		}

	}

	void JexFluidSystem::Tick() {
		grid_ctnr.InsertPaticles(&ptc_buffer);
		ComputeDensityAndPressure();
		ComputeForceAndAcceleration();
		AdvanceParticles();
	}

	void JexFluidSystem::_Init(usht ptc_max_num, const Box & wall_box_, const Box & init_fluid_box, const Vector3f & gravity) {

		ptc_buffer.Reset(ptc_max_num);

		wall_box = wall_box_;
		gravity_direction = gravity;

		/* create the particles */
		float ptc_dist = pow(particle_mass / rest_density, 1.0 / 3.0);
		spacing = ptc_dist / unit_scale;
		AddFluidVolume(init_fluid_box, spacing);

		// setup grid cell size ( 2 * r )
		grid_ctnr.Init(wall_box_, unit_scale, smooth_radius * 2.0, 1.0);
	}

	void JexFluidSystem::ComputeDensityAndPressure() {

		bool nbr_full = false;
		float sr_sq = smooth_radius * smooth_radius;

		// reset neighbor table
		//printf("particle data buffer size : %d\n", ptc_buffer.Size());
		nbr_table.Reset(ptc_buffer.Size());

		for (size_t i = 0; i < ptc_buffer.Size(); i++) {

			Particle * ptc = ptc_buffer.GetParticle(i);

			float sum = 0.0;
			nbr_table.PerpareParticle(i);

			int grid_cell[8];
			grid_ctnr.FindCells(ptc->position, smooth_radius / unit_scale, grid_cell);

			for (size_t cell_index = 0; cell_index < 8; cell_index++) {

				if (grid_cell[cell_index] == -1)
					continue;

				int pndx = grid_ctnr.GetGridData(grid_cell[cell_index]);

				while (pndx != -1) {

					Particle * ptc2 = ptc_buffer.GetParticle(pndx);
					if (ptc == ptc2) {
						sum += pow(sr_sq, 3.0);
					}
					else {

						Vector3f ptc_minus_ptc2 = unit_scale * (ptc->position - ptc2->position);
						float r_sq = ptc_minus_ptc2.LengthSq();

						if (sr_sq > r_sq) {

							sum += pow(sr_sq - r_sq, 3.0);

							if (!nbr_table.AddParticleNeighbor(pndx, sqrt(r_sq))) {
								nbr_full = true;
								break;
							}

						}
					}
					pndx = ptc2->next;
				}

				if (nbr_full)
					break;
			}

			nbr_table.CommitParticle();

			ptc->density = poly6_kernel * particle_mass * sum;
			ptc->pressure = gas_constant * (ptc->density - rest_density);
		}

	}

	void JexFluidSystem::ComputeForceAndAcceleration() {

		float sr_sq = smooth_radius * smooth_radius;

		for (size_t i = 0; i < ptc_buffer.Size(); i++) {

			Particle * ptc = ptc_buffer.GetParticle(i);

			Vector3f acc_sum;
			int nbr_num = nbr_table.GetNeighborNum(i);

			for (size_t j = 0; j < nbr_num; j++) {

				usht nbr_index;

				float r;
				nbr_table.GetNeighborInfo(i, j, nbr_index, r);

				Particle * ptc2 = ptc_buffer.GetParticle(nbr_index);

				// r(i) - r(j)
				Vector3f ri_minus_rj = unit_scale * (ptc->position - ptc2->position);
				// h - r
				float h_r = smooth_radius - r;
				// h * h - r * r
				float h2_r2 = sr_sq - r * r;

				// pressure
				float pterm = -0.5 * spiky_kernel * particle_mass *  h_r * h_r *
					(ptc->pressure + ptc2->pressure) / (ptc->density * ptc2->density);
				acc_sum += ri_minus_rj * pterm / r;

				// viscosity
				float vterm = visco_kernel * viscosity * h_r * particle_mass / (ptc->density * ptc2->density);
				acc_sum += (ptc2->velocity_eval - ptc->velocity_eval) * vterm;
			}

			ptc->acceleration = acc_sum;
		}

	}

	void JexFluidSystem::AdvanceParticles() {

		// fixed delta time per frame
		float delta_time = 0.003;

		float sl_sq = speed_limit * speed_limit;

		for (size_t i = 0; i < ptc_buffer.Size(); i++) {

			Particle * ptc = ptc_buffer.GetParticle(i);

			// compute acceleration
			Vector3f acc = ptc->acceleration;

			// velocity limit
			float acc_sq = acc.LengthSq();
			if (acc_sq > sl_sq) {
				acc *= speed_limit / sqrt(acc_sq);
			}

			/** boundary conditions **/
			// z-axis wall
			float diff = unit_scale * (2 - (ptc->position.Z() - wall_box.MinVertex().Z()));
			if (diff > EPSILON) {
				Vector3f unit_vector(0, 0, 1);
				float adj = boundary_stiffness * diff - boundary_dampening * unit_vector.dot(ptc->velocity_eval);
				acc += adj * unit_vector;
			}
			diff = unit_scale * (2 - (wall_box.MaxVertex().Z() - ptc->position.Z()));
			if (diff > EPSILON) {
				Vector3f unit_vector(0, 0, -1);
				float adj = boundary_stiffness * diff - boundary_dampening * unit_vector.dot(ptc->velocity_eval);
				acc += adj * unit_vector;
			}
			// x-axis wall
			diff = unit_scale * (2 - (ptc->position.X() - wall_box.MinVertex().X()));
			if (diff > EPSILON) {
				Vector3f unit_vector(1, 0, 0);
				float adj = boundary_stiffness * diff - boundary_dampening * unit_vector.dot(ptc->velocity_eval);
				acc += adj * unit_vector;
			}
			diff = unit_scale * (2 - (wall_box.MaxVertex().X() - ptc->position.X()));
			if (diff > EPSILON) {
				Vector3f unit_vector(-1, 0, 0);
				float adj = boundary_stiffness * diff - boundary_dampening * unit_vector.dot(ptc->velocity_eval);
				acc += adj * unit_vector;
			}
			// y-axis wall
			diff = unit_scale * (2 - (ptc->position.Y() - wall_box.MinVertex().Y()));
			if (diff > EPSILON) {
				Vector3f unit_vector(0, 1, 0);
				float adj = boundary_stiffness * diff - boundary_dampening * unit_vector.dot(ptc->velocity_eval);
				acc += adj * unit_vector;
			}
			diff = unit_scale * (2 - (wall_box.MaxVertex().Y() - ptc->position.Y()));
			if (diff > EPSILON) {
				Vector3f unit_vector(0, -1, 0);
				float adj = boundary_stiffness * diff - boundary_dampening * unit_vector.dot(ptc->velocity_eval);
				acc += adj * unit_vector;
			}
			///////////////////////////

			// plane gravity
			acc += gravity_direction;

			/* leapfrog integration */
			// v(t+1/2) = v(t-1/2) + a(t) * dt
			Vector3f vel_next = ptc->velocity + acc * delta_time; 
			// v(t+1) = 0.5 * [v(t-1/2) + v(t+1/2)] used to compute forces later
			ptc->velocity_eval = 0.5 * (ptc->velocity + vel_next);
			ptc->velocity = vel_next;
			// p(t+1) = p(t) + v(t+1/2) * dt
			ptc->position += vel_next * delta_time / unit_scale;

		}

	}

}

//Jex::JexFluidSystem * GetSphSystem() {
//	static Jex::JexFluidSystem jex_system;
//	return &jex_system;
//}