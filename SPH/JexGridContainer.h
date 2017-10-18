#pragma once

#ifndef JEX_GRID_CONTAINER_H
#define JEX_GRID_CONTAINER_H

#include <vector>
#include <cmath>

#include "JexMath.h"

namespace Jex {

	class ParticleBuffer;

	class GridContainer {
	private:
		/* spatial grid */
		std::vector<int> grid_data;
		Vector3f		 grid_min; 
		Vector3f         grid_max;
		Vector3f         grid_res;   // resolution in each axis
		Vector3f         grid_size;  // physical size in each axis
		Vector3f         grid_delta;
		float            grid_cell_size;
	public:
		GridContainer();
		~GridContainer();
		/* spatial subdivision */
		void Init(const Box & box, float sim_scale, float cell_size, float border);
		void InsertPaticles(ParticleBuffer * ptc_buffer);
		void FindCells(const Vector3f & p, float radius, int * grid_cell);
		int FindCell(const Vector3f & p);
		int GetGridData(int grid_index);
		int GetGridCellIndex(float px, float py, float pz);
		const Vector3f * GetGridRes() const { return &grid_res; }
		const Vector3f * GetGridMin() const { return &grid_min; }
		const Vector3f * GetGridMax() const { return &grid_max; }
		const Vector3f * GetGridSize() const { return &grid_size; }
	};

}

#endif
