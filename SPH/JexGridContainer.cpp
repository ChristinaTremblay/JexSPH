
#include "JexGridContainer.h"
#include "JexParticleDataStruct.h"

namespace Jex {

	GridContainer::GridContainer() {

	}

	GridContainer::~GridContainer() {

	}

	int GridContainer::GetGridData(int grid_index) {

		if (grid_index < 0 || grid_index >= (int)grid_data.size())
			return -1;

		return grid_data[grid_index];
	}

	int GridContainer::GetGridCellIndex(float px, float py, float pz) {

		int gx = (int)((px - grid_min.X()) * grid_delta.X());
		int gy = (int)((py - grid_min.Y()) * grid_delta.Y());
		int gz = (int)((pz - grid_min.Z()) * grid_delta.Z());

		return (gz * grid_res.Y() + gy) * grid_res.X() + gx;
	}

	void GridContainer::Init(const Box & box, float sim_scale, float cell_size, float border) {

		/* Ideal grid cell size = 2 * smoothing radius = 2 * 0.02 = 0.04
		*  Ideal domain size = k * gs / d = k * 8 = {8, 16, 24, 32, 40, ... }
		*  k = number of cells, gs = grid cell size, d = simulation scale
		*/
		float world_cell_size = cell_size / sim_scale;

		grid_min = box.MinVertex(); grid_min -= border; //grid_min.print_info();
		grid_max = box.MaxVertex(); grid_max += border; //		grid_max.print_info();
		grid_size = grid_max - grid_min;
		grid_cell_size = world_cell_size;

		// determine grid resolution
		grid_res.set(
			(int)ceil(grid_size.X() / world_cell_size),
			(int)ceil(grid_size.Y() / world_cell_size),
			(int)ceil(grid_size.Z() / world_cell_size));
		//grid_res.print_info();
		// adjust grid size to multiple of cell size
		grid_size.set(
			grid_res.X() * cell_size / sim_scale,
			grid_res.Y() * cell_size / sim_scale,
			grid_res.Z() * cell_size / sim_scale);
		// delta = translate from world space to cell
		grid_delta = grid_res / grid_size;

		int total_grid_num = (int)(grid_res.X() * grid_res.Y() * grid_res.Z());
		grid_data.resize(total_grid_num);
	}

	void GridContainer::InsertPaticles(ParticleBuffer * ptc_buffer) {

		std::fill(grid_data.begin(), grid_data.end(), -1);

		Particle * ptc = ptc_buffer->GetParticle(0);
		for (size_t i = 0; i < ptc_buffer->Size(); i++, ptc++) {

			float px = ptc->position.X(); 
			float py = ptc->position.Y();
			float pz = ptc->position.Z();

			int grid_cell_index = GetGridCellIndex(px, py, pz);
			if (0 <= grid_cell_index && grid_cell_index <= grid_data.size()) {
				ptc->next = grid_data[grid_cell_index];
				grid_data[grid_cell_index] = (int)i;
			}
			else {
				ptc->next = -1;
			}

		}

	}

	int GridContainer::FindCell(const Vector3f & p) {

		int grid_cell_index = GetGridCellIndex(p.X(), p.Y(), p.Z());

		if (grid_cell_index < 0 || grid_cell_index >= (int)grid_data.size())
			return -1;
		return grid_cell_index;

	}

	void GridContainer::FindCells(const Vector3f & p, float radius, int * grid_cell) {

		for (size_t i = 0; i < 8; i++)
			grid_cell[i] = -1;

		// compute sphere range
		int sphere_min_x = Max((int)(grid_delta.X() * (p.X() - grid_min.X() - radius)), 0);
		int sphere_min_y = Max((int)(grid_delta.Y() * (p.Y() - grid_min.Y() - radius)), 0);
		int sphere_min_z = Max((int)(grid_delta.Z() * (p.Z() - grid_min.Z() - radius)), 0);

		grid_cell[0] = (sphere_min_z * grid_res.Y() + sphere_min_y) * grid_res.X() + sphere_min_x;
		grid_cell[1] = grid_cell[0] + 1;
		grid_cell[2] = (int)(grid_cell[0] + grid_res.X());
		grid_cell[3] = grid_cell[2] + 1;

		if (sphere_min_z + 1 < grid_res.Z()) {
			grid_cell[4] = (int)(grid_cell[0] + grid_res.Y() * grid_res.X());
			grid_cell[5] = grid_cell[4] + 1;
			grid_cell[6] = (int)(grid_cell[4] + grid_res.X());
			grid_cell[7] = grid_cell[6] + 1;
		}
		if (sphere_min_x + 1 >= grid_res.X()) {
			grid_cell[1] = -1;
			grid_cell[3] = -1;
			grid_cell[5] = -1;
			grid_cell[7] = -1;
		}
		if (sphere_min_y + 1 >= grid_res.Y()) {
			grid_cell[2] = -1;
			grid_cell[3] = -1;
			grid_cell[6] = -1;
			grid_cell[7] = -1;
		}

	}

}