#pragma once

#ifndef JEX_SPH_INTERFACE_H
#define JEX_SPH_INTERFACE_H

#include "JexMath.h"

namespace Jex {

	class JexSystem {
	public:
		virtual void Init(
			unsigned short max_ptc_num,
			const Float3 * wall_box_min,
			const Float3 * wall_box_max,
			const Float3 * init_fluid_box_min,
			const Float3 * init_fluid_box_max,
			const Float3 * gravity
		) = 0;
		virtual unsigned int GetParticleStride() const = 0;
		virtual unsigned int GetParticleNum() const = 0;
		virtual const Float3 * GetParticleBuffer() const = 0;
		virtual void Tick() = 0;
	};

	//extern "C" {
	//	/* get the sigleton SPH system particle */
	//	Jex::JexFluidSystem * GetSphSystem();
	//}

}

#endif