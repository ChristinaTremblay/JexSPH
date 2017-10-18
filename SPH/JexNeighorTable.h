#pragma once

#ifndef JEX_NEIGHBOR_TABLE_H
#define JEX_NEIGHBOR_TABLE_H

namespace Jex {

#define uint unsigned int
#define usht unsigned short
#define uchr unsigned char

	class NeighborTable{
	private:
		enum {
			NBR_MAX_NUM = 80
		};
	
		union ParticleExtraData {
			struct {
				unsigned nbr_data_offset : 24;
				unsigned nbr_num : 8;
			};
			uint nbr_data;
		};

		ParticleExtraData * ptc_extra_data;
		uint ptc_num;
		uint ptc_capacity;

		uchr * nbr_data_buffer; // neighbor data buffer
		uint data_buffer_size;			 // in bytes
		uint data_buffer_offset;		 // current neighbor data buffer offset

		/* temporary data for current particle */
		usht curr_ptc_index;
		int curr_nbr_num;
		usht curr_nbr_index[NBR_MAX_NUM];
		float curr_nbr_dist[NBR_MAX_NUM];

		void GrowDataBuffer(uint need_size);

	public:
		NeighborTable();
		~NeighborTable();

		/* reset neighbor table */
		void Reset(usht ptc_num);
		/* prepare a particle neighbor data */
		void PerpareParticle(usht ptc_index);
		/* add neighbor data to current particle */ 
		bool AddParticleNeighbor(usht ptc_index, float dist);
		/* commit particle neighbor data to data buffer */
		void CommitParticle();
		/* get particle neighbor num */
		int GetNeighborNum(usht ptc_index) {
			return ptc_extra_data[ptc_index].nbr_num;
		}
		/* get particle neighbor information */
		void GetNeighborInfo(usht ptc_index, int index, usht & nbr_index, float & nbr_dist);
	};

}



#endif
