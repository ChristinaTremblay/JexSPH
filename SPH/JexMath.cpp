
#include "JexMath.h"

namespace Jex {

#pragma region Float3

	Float3::Float3() {
		x = y = z = 0.0;
	}

	Float3::Float3(float x_, float y_, float z_) {
		x = x_; y = y_; z = z_;
	}

	Float3::Float3(const Vector3f v) {
		x = v.X(); y = v.Y(); z = v.Z();
	}

	Float3::Float3(const Vector3f & v) {
		x = v.X(); y = v.Y(); z = v.Z();
	}

	Float3::Float3(const Vector3f * v) {
		x = v->X(); y = v->Y(); z = v->Z();
	}

#pragma endregion

#pragma region Float4

	Float4::Float4() {
		x = y = z = w = 0.0;
	}

	Float4::Float4(float x_, float y_, float z_, float w_) {
		x = x_; y = y_; z = z_; w = w_;
	}

	Float4::Float4(const Float4 & f4) {
		x = f4.x; y = f4.y; z = f4.z; w = f4.w;
	}

	Float4::Float4(const Float4 * f4) {
		x = f4->x; y = f4->y; z = f4->z; w = f4->w;
	}

	Vector3f Float4::TransforToVector3f() const {
		if (w < EPSILON) {
			return Vector3f(x, y, z);
		}
		else {
			return Vector3f(x / w, y / w, z / w);
		}
	}

	void Float4::print_info() const {
		printf("x = %f, y = %f, z = %f, w = %f\n", x, y, z, w);
	}

#pragma endregion

#pragma region Float16

	Float16::Float16() {

	}

#pragma omp parallel for
	Float16::Float16(const Float16 & f16) {
		for (size_t i = 0; i < 16; i++) {
			data[i] = f16.data[i];
		}
	}

#pragma omp parallel for
	Float16::Float16(const Float16 * f16) {
		for (size_t i = 0; i < 16; i++) {
			data[i] = f16->data[i];
		}
	}

#pragma omp parallel for
	Float16::Float16(const float(&data_)[16]) {
		for (size_t i = 0; i < 16; i++) {
			data[i] = data_[i];
		}
	}

	Float16::~Float16() {

	}

	float Float16::operator [](int i) const {
		return data[i];
	}

#pragma endregion

#pragma region Vector3f

	Float4 Vector3f::TransforToHomoCoord(float w) const {
		return Float4(x, y, z, w);
	}

	Vector3f Vector3f::cross(const Vector3f & v) const {
		return Vector3f(y * v.Z() - z * v.Y(),
			x * v.Z() - z * v.X(),
			x * v.Y() - y * v.X());
	}

#pragma endregion

#pragma region Matrix4x4f

	Matrix4x4f::Matrix4x4f() {

	}

#pragma omp parallel for
	Matrix4x4f::Matrix4x4f(const Matrix4x4f & mtrx) {
		Float16 data_ = mtrx.GetData();
		for (size_t i = 0; i < 16; i++) {
			data[i] = data_.data[i];
		}
	}

#pragma omp parallel for
	Matrix4x4f::Matrix4x4f(const Matrix4x4f * mtrx) {
		Float16 data_ = mtrx->GetData();
		for (size_t i = 0; i < 16; i++) {
			data[i] = data_.data[i];
		}
	}

#pragma omp parallel for
	Matrix4x4f::Matrix4x4f(const float (&data_)[16]) {
		for (size_t i = 0; i < 16; i++) {
			data[i] = data_[i];
		}
	}

	Matrix4x4f::~Matrix4x4f() {

	}

	float Matrix4x4f::GetData(int i) const {
		return data[i];
	}

	Float16 Matrix4x4f::GetData() const {
		return Float16(data);
	}

	void Matrix4x4f::SetData(int i, float f) {
		if (0 <= i && i < 16) {
			data[i] = f;
		}
	}

	void Matrix4x4f::SetRowData(int r, const Float4 & f4) {
		if (0 <= r && r < 4) {
			data[r] = f4.x;
			data[r + 1] = f4.y;
			data[r + 2] = f4.z;
			data[r + 3] = f4.w;
		}
	}

	void Matrix4x4f::SetColumnData(int c, const Float4 & f4) {
		if (0 <= c && c < 4) {
			data[c] = f4.x;
			data[c + 4] = f4.y;
			data[c + 8] = f4.z;
			data[c + 12] = f4.w;
		}
	}

	bool Matrix4x4f::operator == (const Matrix4x4f & mtrx) {
		Float16 data_ = mtrx.GetData();
		for (size_t i = 0; i < 16; i++) {
			if (data[i] != data_[i]) {
				return false;
			}
		}
		return true;
	}

#pragma omp parallel for
	Matrix4x4f & Matrix4x4f::operator = (const Matrix4x4f & mtrx) {
		Float16 data_ = mtrx.GetData();
		for (size_t i = 0; i < 16; i++) {
			data[i] = data_[i];
		}
		return *this;
	}

#pragma omp parallel for
	Matrix4x4f & Matrix4x4f::operator += (const Matrix4x4f & mtrx) {
		Float16 data_ = mtrx.GetData();
		for (size_t i = 0; i < 16; i++) {
			data[i] += data_[i];
		}
		return *this;
	}

#pragma omp parallel for
	Matrix4x4f & Matrix4x4f::operator -= (const Matrix4x4f & mtrx) {
		Float16 data_ = mtrx.GetData();
		for (size_t i = 0; i < 16; i++) {
			data[i] -= data_[i];
		}
		return *this;
	}

#pragma omp parallel for
	Matrix4x4f & Matrix4x4f::operator *= (const Matrix4x4f & mtrx) {
		Float16 data_ = mtrx.GetData();
		Float16 old_data = this->GetData();
		size_t k = 0;
		for(size_t i = 0; i < 4; i++){
			for (size_t j = 0; j < 4; j++) {
				data[k] = 0.0;
				data[k] += (old_data[i * 4] * data_[j]
						+ old_data[i * 4 + 1] * data_[j + 4]
						+ old_data[i * 4 + 2] * data_[j + 8]
						+ old_data[i * 4 + 3] * data_[j + 12]);
				k++;
			}
		}
		return *this;
	}

	float & Matrix4x4f::operator [] (int i) {
		return data[i];
	}
	
	void Matrix4x4f::print_info() const {
		printf("%f %f %f %f\n", data[0], data[1], data[2], data[3]);
		printf("%f %f %f %f\n", data[4], data[5], data[6], data[7]);
		printf("%f %f %f %f\n", data[8], data[9], data[10], data[11]);
		printf("%f %f %f %f\n", data[12], data[13], data[14], data[15]);
	}

#pragma endregion

#pragma region OverloadOperators

	

	
#pragma endregion

#pragma region MatrixTransformations

	float RadianToDegree(float radian) {
		return 180.0 * radian / PI;
	}

	float DegreeToRadian(float degree) {
		return PI * degree / 180.0;
	}

	Matrix4x4f Translate(float dx, float dy, float dz) {

		Matrix4x4f trans_mtrx = ProductIdentityMtrx();

		trans_mtrx.SetData(3, dx);
		trans_mtrx.SetData(7, dy);
		trans_mtrx.SetData(11, dz);

		return trans_mtrx;
	}

	Matrix4x4f TranslateX(float dx) {
		return Translate(dx, 0.0, 0.0);
	}

	Matrix4x4f TranslateY(float dy) {
		return Translate(0.0, dy, 0.0);
	}

	Matrix4x4f TranslateZ(float dz) {
		return Translate(0.0, 0.0, dz);
	}

	Matrix4x4f Scale(float dx, float dy, float dz, Float4 p, Float4 o) {

		// p 到 o 的平移矩阵
		Matrix4x4f po_mtrx = Translate(o.x - p.x, o.y - p.y, o.z - p.z);

		// 缩放矩阵
		Matrix4x4f scale_mtrx = ProductIdentityMtrx();
		scale_mtrx.SetData(0, dx);
		scale_mtrx.SetData(5, dy);
		scale_mtrx.SetData(10, dz);

		// o 到 p 的平移矩阵
		Matrix4x4f op_mtrx = Translate(p.x - o.x, p.y - o.y, p.z - o.z);

		return op_mtrx * scale_mtrx * po_mtrx;
	}

	Matrix4x4f Scale(float ds, Float4 p, Float4 o) {
		return Scale(ds, ds, ds, p, o);
	}

	Matrix4x4f ScaleX(float dx, Float4 p, Float4 o) {
		return Scale(dx, 0.0, 0.0, p, o);
	}

	Matrix4x4f ScaleY(float dy, Float4 p, Float4 o) {
		return Scale(0.0, dy, 0.0, p, o);
	}

	Matrix4x4f ScaleZ(float dz, Float4 p, Float4 o) {
		return Scale(0.0, 0.0, dz, p, o);
	}

	Matrix4x4f Rotate(float dx, float dy, float dz, Float4 p, Float4 o) {

		// p 到 o 的平移矩阵
		Matrix4x4f po_mtrx = Translate(o.x - p.x, o.y - p.y, o.z - p.z);

		// 沿自身 x 轴的旋转矩阵
		Matrix4x4f rx_mtrx = ProductIdentityMtrx();
		if (abs(dx) > EPSILON) {
			rx_mtrx.SetData(5, cos(DegreeToRadian(dx)));
			rx_mtrx.SetData(6, -sin(DegreeToRadian(dx)));
			rx_mtrx.SetData(9, sin(DegreeToRadian(dx)));
			rx_mtrx.SetData(10, cos(DegreeToRadian(dx)));
		}

		// 沿自身 y 轴的旋转矩阵
		Matrix4x4f ry_mtrx = ProductIdentityMtrx();
		if (abs(dy) > EPSILON) {
			ry_mtrx.SetData(0, cos(DegreeToRadian(dy)));
			ry_mtrx.SetData(2, -sin(DegreeToRadian(dy)));
			ry_mtrx.SetData(8, sin(DegreeToRadian(dy)));
			ry_mtrx.SetData(10, cos(DegreeToRadian(dy)));
		}

		// 沿自身 z 轴的旋转矩阵
		Matrix4x4f rz_mtrx = ProductIdentityMtrx();
		if (abs(dz) > EPSILON) {
			rz_mtrx.SetData(0, cos(DegreeToRadian(dz)));
			rz_mtrx.SetData(1, -sin(DegreeToRadian(dz)));
			rz_mtrx.SetData(4, sin(DegreeToRadian(dz)));
			rz_mtrx.SetData(5, cos(DegreeToRadian(dz)));
		}

		// o 到 p 的平移矩阵
		Matrix4x4f op_mtrx = Translate(p.x - o.x, p.y - o.y, p.z - o.z);

		return op_mtrx * rz_mtrx * ry_mtrx * rx_mtrx * po_mtrx;
	}


	Matrix4x4f RotateX(float dx, Float4 p, Float4 o) {
		return Rotate(dx, 0.0, 0.0, p, o);
	}


	Matrix4x4f RotateY(float dy, Float4 p, Float4 o) {
		return Rotate(0.0, dy, 0.0, p, o);
	}


	Matrix4x4f RotateZ(float dz, Float4 p, Float4 o) {
		return Rotate(0.0, 0.0, dz, p, o);
	}

#pragma endregion

}