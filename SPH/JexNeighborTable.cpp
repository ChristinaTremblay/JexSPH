
#include "JexNeighorTable.h"

#include <cstring>
#include <stdlib.h>

namespace Jex {

	NeighborTable::NeighborTable() 
		:ptc_extra_data(0)
		,ptc_num(0)
		,ptc_capacity(0)
		,nbr_data_buffer(0)
		,data_buffer_size(0)
		,curr_nbr_num(0)
		,curr_ptc_index(0)
		,data_buffer_offset(0){}

	NeighborTable::~NeighborTable() {
		
		if (ptc_extra_data)
			free(ptc_extra_data);

		if (nbr_data_buffer)
			free(nbr_data_buffer);

	}

	void NeighborTable::Reset(usht new_ptc_num) {

		int num_byte = sizeof(ptc_extra_data);

		if (new_ptc_num > ptc_capacity) {
			if(ptc_extra_data)
				free(ptc_extra_data);

			ptc_extra_data = (ParticleExtraData*)malloc(new_ptc_num * sizeof(ptc_extra_data));
			ptc_capacity = new_ptc_num;
		}
			
		ptc_num = new_ptc_num;
		memset(ptc_extra_data, 0, ptc_capacity * sizeof(ptc_extra_data));
		data_buffer_offset = 0;
	}

	void NeighborTable::PerpareParticle(usht ptc_index_) {
		curr_ptc_index = ptc_index_;
		curr_nbr_num = 0;
	}

	bool NeighborTable::AddParticleNeighbor(usht ptc_index_, float dist) {

		if (curr_nbr_num >= NBR_MAX_NUM)
			return false;

		curr_nbr_index[curr_nbr_num] = ptc_index_;
		curr_nbr_dist[curr_nbr_num] = dist;
		curr_nbr_num++;

		return true;
	}

	void NeighborTable::CommitParticle() {

		if (curr_nbr_num == 0)
			return;

		uint index_size = curr_nbr_num * sizeof(usht);
		uint dist_size = curr_nbr_num * sizeof(float);

		// grow particle data buffer
		uint curr_buffer_size = data_buffer_offset + index_size + dist_size;
		if (curr_buffer_size > data_buffer_size)
			GrowDataBuffer(curr_buffer_size);

		// set neighbor data
		ptc_extra_data[curr_ptc_index].nbr_num = curr_nbr_num;
		ptc_extra_data[curr_ptc_index].nbr_data_offset = data_buffer_offset;

		// copy index data
		memcpy(nbr_data_buffer + data_buffer_offset, curr_nbr_index, index_size);
		data_buffer_offset += index_size;

		// copy distance data
		memcpy(nbr_data_buffer + data_buffer_offset, curr_nbr_dist, dist_size);
		data_buffer_offset += dist_size;

	}

	void NeighborTable::GrowDataBuffer(uint need_size) {

		uint new_size = data_buffer_size > 0 ? data_buffer_size : 1;
		
		while (new_size < need_size) 
			new_size *= 2;

		if (new_size < 1024)
			new_size = 1024;

		uchr * new_buffer = (uchr*)malloc(new_size);

		if (nbr_data_buffer) {
			memcpy(new_buffer, nbr_data_buffer, data_buffer_size);
			free(nbr_data_buffer);
		}

		nbr_data_buffer = new_buffer;
		data_buffer_size = new_size;

	}

	void NeighborTable::GetNeighborInfo(usht ptc_index, int index, usht & nbr_index, float & nbr_dist) {

		ParticleExtraData ptc_nbr_data = ptc_extra_data[ptc_index];

		usht * index_buffer = (usht*)(nbr_data_buffer + ptc_nbr_data.nbr_data_offset);
		float * dist_buffer = (float*)(nbr_data_buffer + ptc_nbr_data.nbr_data_offset +
							ptc_nbr_data.nbr_num * sizeof(usht));

		nbr_index = index_buffer[index];
		nbr_dist = dist_buffer[index];

	}

}