#pragma once

#ifndef JEX_CAMERA_H
#define JEX_CAMERA_H

#include "JexMath.h"

namespace Jex {

	class JexCamera {
	private:
		Float4 location;
		Float4 focus;
		Float4 up;
		Mtx4f view_mtrx;
		Mtx4f model_mtrx;
		Mtx4f projection_mtrx;
	public:
		JexCamera();
		JexCamera(const JexCamera & camera);	
		~JexCamera();
		
		void Ortho(const Float3 & lbn, const Float3 & rtf);
		void Frustum();

		void MoveTo(const Float4 & location_);
		void MoveTo(float x_, float y_, float z_);

		void MoveBy(const Float4 & delta);
		void MoveBy(float x_, float y_, float z_);

		void LookAt(const Float4 & focus_);
		void LookAt(float x_, float y_, float z_);

		void RotateUp(const Float4 & up_);
		void RotateUp(float x_, float y_, float z_);

		void CalculateViewMtrx();

		Float4 GetLocation() const { return location; }
		Float4 GetFocus() const { return focus; }
		Float4 GetUp() const { return up; }
		Mtx4f GetViewMtrx() const { return view_mtrx; }
		Mtx4f GetModelMtrx() const { return model_mtrx; }
		Mtx4f GetProjectionMtrx() const { return projection_mtrx; }

		void SetViewMtrx(const Mtx4f & view_mtrx_) { view_mtrx = view_mtrx_; }
		void SetModelMtrx(const Mtx4f & model_mtrx_) { model_mtrx = model_mtrx_; }
		void SetProjectionMtrx(const Mtx4f & projection_mtrx_) {
			projection_mtrx = projection_mtrx_;
		}
	};

}

#endif