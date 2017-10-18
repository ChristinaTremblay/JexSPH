#pragma once

#ifndef JEX_FLUID_SYSTEM_H
#define JEX_FLUID_SYSTEM_H

#include "JexParticleDataStruct.h"
#include "JexGridContainer.h"
#include "JexNeighorTable.h"
#include "JexSphInterface.h"

namespace Jex {

	class JexFluidSystem : public JexSystem {
	private:
		ParticleBuffer ptc_buffer;
		GridContainer grid_ctnr;
		NeighborTable nbr_table;

		// SPH Kernel
		float poly6_kernel;
		float spiky_kernel;
		float visco_kernel;

		float unit_scale;
		float spacing;
		float viscosity;
		float rest_density;
		float particle_mass;
		float smooth_radius;
		float gas_constant;
		float boundary_stiffness;
		float boundary_dampening;
		float speed_limit;
		Vector3f gravity_direction;
		Box wall_box;
	private:
		/* 初始化系统 */
		void _Init(usht ptc_max_num, const Box & wall_box_, const Box & init_fluid_box, const Vector3f & gravity);
		/* 创建初始液体块 */
		void AddFluidVolume(const Box & fluid_box, float spacing);
		/* 计算粒子密度和压强 */
		void ComputeDensityAndPressure();
		/* 计算粒子受力及加速度 */
		void ComputeForceAndAcceleration();
		/* 移动粒子 */
		void AdvanceParticles();
	public:
		JexFluidSystem();
		~JexFluidSystem();

		virtual void Init(
			unsigned short max_ptc_num,
			const Float3 * wall_box_min,
			const Float3 * wall_box_max,
			const Float3 * init_fluid_box_min,
			const Float3 * init_fluid_box_max,
			const Float3 * gravity
		) {
			_Init(
				max_ptc_num,
				Box(Vector3f(wall_box_min), Vector3f(wall_box_max)),
				Box(Vector3f(init_fluid_box_min), Vector3f(init_fluid_box_max)),
				Vector3f(gravity)
			);
		}
		/* get the size of particle in bytes */
		virtual uint GetParticleStride() const { return sizeof(Particle); }
		/* get the number of paritcles */
		virtual uint GetParticleNum() const { return ptc_buffer.Size(); }
		/* get the fluid particle buffer */
		virtual const Float3 * GetParticleBuffer() const { 
			return (const Float3*)ptc_buffer.GetParticle(0);
		}
		const Particle * GetParticles() const {
			return ptc_buffer.GetParticle(0);
		}
		/* 逻辑帧 */
		virtual void Tick();

		float GetUnitScale() const { return unit_scale; }
		float GetSpacing() const { return spacing; }
		float GetViscosity() const { return viscosity; }
		float GetRestDensity() const { return rest_density; }
		float GetParticleMass() const { return particle_mass; }
		float GetSmoothRadius() const { return smooth_radius; }
		float GetGasConstant() const { return gas_constant; }
		float GetBoundaryStiffness() const { return boundary_stiffness; }
		float GetBoundaryDampening() const { return boundary_dampening; }
		float GetSpeedLimit() const { return speed_limit; }

		void SetUnitScale(float unit_scale_) { unit_scale = unit_scale_; }
		void SetSpacing(float spacing_) { spacing = spacing_; }
		void SetViscosity(float viscosity_) { viscosity = viscosity_; }
		void SetRestDensity(float rest_density_) { rest_density = rest_density_; }
		void SetParticleMass(float particle_mass_) { particle_mass = particle_mass_; }
		void SetSmoothRadius(float smooth_radius_) { smooth_radius = smooth_radius_; }
		void SetGasConstant(float gas_constant_) { gas_constant = gas_constant_; }
		void SetBoundaryStiffness(float bs) { boundary_stiffness = bs; }
		void SetBoundaryDampening(float bd) { boundary_dampening = bd; }
		void SetSpeedLimit(float speed_limit_) { speed_limit = speed_limit_; }

	};

}

#endif