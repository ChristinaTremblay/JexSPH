
#include "JexParticleDataStruct.h"

#include <cstring>

namespace Jex {

	void ParticleBuffer::Reset(uint capacity) {

		buffer_capacity = capacity;

		if (particle_buffer != 0x0) {
			free(particle_buffer); particle_buffer = 0x0;
		}

		if (buffer_capacity > 0) {
			particle_buffer = (Particle*)malloc(buffer_capacity * sizeof(Particle));
		}
		particle_num = 0;

	}


	Particle * ParticleBuffer::AddParticleReuse() {

		if (particle_num >= buffer_capacity) {

			// get a random particle
			if (buffer_capacity * 2 > PTC_MAX_NUM) {
				uint i = rand() % particle_num;
				return particle_buffer + i;
			}

			// remalloc particle buffer
			buffer_capacity *= 2;
			Particle * new_particle_buffer = (Particle*)malloc(buffer_capacity * sizeof(Particle));
			memcpy(new_particle_buffer, particle_buffer, particle_num * sizeof(Particle));
			free(particle_buffer);
			particle_buffer = new_particle_buffer;

		}

		Particle * p = particle_buffer + (particle_num++);

		p->density = 0;
		p->pressure = 0;
		p->next = 0;
		p->position.set(0, 0, 0);
		p->acceleration.set(0, 0, 0);
		p->velocity.set(0, 0, 0);
		p->velocity_eval.set(0, 0, 0);

		return p;
	}
}