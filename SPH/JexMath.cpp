
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

	void Matrix4x4f::print_info() const {
		printf("%f %f %f %f\n", data[0], data[1], data[2], data[3]);
		printf("%f %f %f %f\n", data[4], data[5], data[6], data[7]);
		printf("%f %f %f %f\n", data[8], data[9], data[10], data[11]);
		printf("%f %f %f %f\n", data[12], data[13], data[14], data[15]);
	}

#pragma endregion

#pragma region OverloadOperators

	

	
#pragma endregion

}