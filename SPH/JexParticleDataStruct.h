#pragma once

#ifndef JEX_PARTICLE_DATA_STRUCT_H
#define JEX_PARTICLE_DATA_STRUCT_H

#include "JexMath.h"

namespace Jex {

#define uint unsigned int

	struct Particle {
	public:
		float density;         // �ܶ�
		float pressure;        // ѹǿ
		Vector3f position;     // λ��
		Vector3f acceleration; // ���ٶ�
		Vector3f velocity;	   // �ٶ�
		Vector3f velocity_eval;
		int next;
	};

	class ParticleBuffer {
	private:
		enum {
			PTC_MAX_NUM = 4096
		};
		Particle * particle_buffer;
		uint particle_num;
		uint buffer_capacity;
	public:
		ParticleBuffer() {
			particle_buffer = 0x0; particle_num = 0; buffer_capacity = 0;
		}
		virtual ~ParticleBuffer() {
			if (particle_buffer) 
				free(particle_buffer);
			particle_buffer = 0;
		}

		uint Size() const { return particle_num; }
		Particle * GetParticle(uint i) { return particle_buffer + i; }
		const Particle * GetParticle(uint i) const { return particle_buffer + i; }
		void Reset(uint capacity);
		Particle * AddParticleReuse();
	};

}


#endif
