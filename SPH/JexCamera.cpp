
#include "JexCamera.h"

namespace Jex {

	JexCamera::JexCamera() {
		location = Float4(0.0, 0.0, 0.0, 1.0);
		focus = Float4(0.0, 0.0, -1.0, 1.0);
		up = Float4(0.0, 1.0, 0.0, 0.0);
		eye = Float4(0.0, 0.0, 0.0, 1.0);
		at = Float4(0.0, 0.0, -1.0, 1.0);
		view_mtrx = ProductIdentityMtrx();
		model_mtrx = ProductIdentityMtrx();
		projection_mtrx = ProductIdentityMtrx();
	}

	JexCamera::JexCamera(const JexCamera & camera) {
		location = camera.GetLocation();
		focus = camera.GetFocus();
		up = camera.GetUp();
		view_mtrx = camera.GetViewMtrx();
		model_mtrx = camera.GetModelMtrx();
		projection_mtrx = camera.GetProjectionMtrx();
	}

	JexCamera::~JexCamera() {

	}

	Mtx4f JexCamera::Ortho(const Float3 & lbn, const Float3 & rtf) {

		float left = lbn.x;
		float bottom = lbn.y;
		float near = -lbn.z;

		float right = rtf.x;
		float top = rtf.y;
		float far = -rtf.z;

		float dx = right - left;
		float dy = top - bottom;
		float dz = far - near;

		if (abs(dx) < EPSILON || abs(dy) < EPSILON || abs(dz) < EPSILON) {
			// nothing change
		}
		else {
			projection_mtrx.SetData(0, 2.0 / dx);
			projection_mtrx.SetData(3, -(left + right) / dx);
			projection_mtrx.SetData(5, 2.0 / dy);
			projection_mtrx.SetData(7, -(top + bottom) / dy);
			projection_mtrx.SetData(10, -2.0 / dz);
			projection_mtrx.SetData(11, -(far + near) / dz);
		}

		return projection_mtrx;
	}

	Mtx4f JexCamera::Frustum (const Float3 & lbn, const Float3 & rtf) {

		float left = lbn.x;
		float bottom = lbn.y;
		float near = -lbn.z;

		float right = rtf.x;
		float top = rtf.y;
		float far = -rtf.z;

		float dx = right - left;
		float dy = top - bottom;
		float dz = far - near;

		if (abs(dx) < EPSILON || abs(dy) < EPSILON || abs(dz) < EPSILON) {
			// nothing change
		}
		else {
			projection_mtrx.SetData(0, 2.0 / dx);
			projection_mtrx.SetData(3, -(left + right) / dx);
			projection_mtrx.SetData(5, 2.0 / dy);
			projection_mtrx.SetData(7, -(top + bottom) / dy);
			projection_mtrx.SetData(10, -2.0 / dz);
			projection_mtrx.SetData(11, -(far + near) / dz);
		}

		return projection_mtrx;
	}

	void JexCamera::MoveTo(const Float4 & location_) {
		location = location_;
	}

	void JexCamera::MoveTo(float x_, float y_, float z_) {
		location.x = x_;
		location.y = y_;
		location.z = z_;
	}

	void JexCamera::MoveBy(const Float4 & delta) {
		location += delta;
	}

	void JexCamera::MoveBy(float x_, float y_, float z_) {
		location.x += x_;
		location.y += y_;
		location.z += z_;
	}

	void JexCamera::LookAt(const Float4 & focus_) {
		focus = focus_;
	}

	void JexCamera::LookAt(float x_, float y_, float z_) {
		focus.x = x_; focus.y = y_; focus.z = z_;
	}

	Mtx4f JexCamera::LookAt(const Float4 & eye_, const Float4 & at_, const Float4 & up_) {

		eye = eye_; at = at_; up = up_;

		Vector3f n = (at - eye).TransforToVector3f();
		n.Normalize();

		Vector3f u = up.TransforToVector3f().cross(n);
		u.Normalize();
		
		Vector3f v = n.cross(u);
		v.Normalize();
	
		Float4 r1 = u.TransforToHomoCoord(-(at.x * u.X() + at.y * u.Y() + at.z * u.Z()));
		Float4 r2 = v.TransforToHomoCoord(-(at.x * v.X() + at.y * v.Y() + at.z * v.Z()));
		Float4 r3 = n.TransforToHomoCoord(-(at.x * n.X() + at.y * n.Y() + at.z * n.Z()));
		Float4 r4 = Float4(0.0, 0.0, 0.0, 1.0);

		view_mtrx.SetRowData(0, r1);
		view_mtrx.SetRowData(4, r2);
		view_mtrx.SetRowData(8, r3);
		view_mtrx.SetRowData(12, r4);

		return view_mtrx;
	}
	
	void JexCamera::RotateUp(const Float4 & up_) {
		up = up_;
	}

	void JexCamera::RotateUp(float x_, float y_, float z_) {
		up.x = x_; up.y = y_; up.z = z_;
	}

	void JexCamera::CalculateViewMtrx() {

		Vector3f n = (focus - location).TransforToVector3f();
		n /= n.Length();
		n.print_info();

		Vector3f u = up.TransforToVector3f().cross(n);
		u /= u.Length();
		u.print_info();

		Vector3f v = n.cross(u);
		v /= v.Length();
		v.print_info();

		Float4 r1 = u.TransforToHomoCoord(-(focus.x * u.X() + focus.y * u.Y() + focus.z * u.Z()));
		Float4 r2 = v.TransforToHomoCoord(-(focus.x * v.X() + focus.y * v.Y() + focus.z * v.Z()));
		Float4 r3 = n.TransforToHomoCoord(-(focus.x * n.X() + focus.y * n.Y() + focus.z * n.Z()));
		Float4 r4 = Float4(0.0, 0.0, 0.0, 1.0);

		view_mtrx.SetRowData(0, r1);
		view_mtrx.SetRowData(1, r2);
		view_mtrx.SetRowData(2, r3);
		view_mtrx.SetRowData(3, r4);

	}

}
