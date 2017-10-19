#pragma once

#ifndef JEX_MATH_H
#define JEX_MATH_H

#include <omp.h>
#include <cmath>
#include <stdio.h>

namespace Jex {

#define EPSILON 0.000001
#define PI 3.1415926535

#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Min(a, b) (((a) < (b)) ? (a) : (b))

	class Float3;
	class Vector3f;

#pragma region Float3

	class Float3 {
	public:
		float x, y, z;
		Float3();
		Float3(float x_, float y_, float z_);
		Float3(const Vector3f v);
		Float3(const Vector3f & v);
		Float3(const Vector3f * v);
		~Float3() {}
	};

#pragma endregion

#pragma region Float4

	// 主要用于矩阵变换用
	class Float4 {
	public:
		float x, y, z, w;
		Float4();
		Float4(float x_, float y_, float z_, float w_);
		Float4(const Float4 & f4);
		Float4(const Float4 * f4);
		~Float4() {}

		void print_info() const;
	};

#pragma endregion

#pragma region Float16

	class Float16 {
	public:
		float data[16] = { 0.0 };
		Float16();
		Float16(const Float16 & f16);
		Float16(const Float16 * f16);
		Float16(const float (&data_)[16]);
		~Float16();

		float operator [](int i) const;
	};

#pragma endregion

#pragma region Vector3f

	class Vector3f {

	private:

		float x, y, z;

	public:

		Vector3f() :x(0.0), y(0.0), z(0.0) {}
		Vector3f(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
		Vector3f(const Vector3f & v) {
			x = v.X(); y = v.Y(); z = v.Z();
		}
		Vector3f(const Float3 * fs) :x(fs->x), y(fs->y), z(fs->z) {}
		~Vector3f() {}

		void set(float x_, float y_, float z_) {
			x = x_; y = y_; z = z_;
		}

		float X() const { return x; }
		float Y() const { return y; }
		float Z() const { return z; }
		void SetX(float x_) { x = x_; }
		void SetY(float y_) { y = y_; }
		void SetZ(float z_) { z = z_; }

		float dot(const Vector3f & v) const {
			return x * v.X() + y * v.Y() + z * v.Z();
		}

		float LengthSq() const { return x*x + y*y + z*z; }
		float Length() const { return sqrt(LengthSq()); }

		void Normalize() {
			if (LengthSq() > EPSILON) {
				float len = Length();
				x /= len; y /= len; z /= len;
			}
		}

		// 转换成齐次坐标
		Float4 TransforToHomoCoord(float w) const;

		bool operator == (const Vector3f & v) {
			return x == v.x && y == v.y && z == v.z;
		}

		Vector3f & operator += (float f) {
			x += f; y += f; z += f; return *this;
		}
		Vector3f & operator -= (float f) {
			x -= f; y -= f; z -= f; return *this;
		}
		Vector3f & operator *= (float f) {
			x *= f; y *= f; z *= f; return *this;
		}
		Vector3f & operator /= (float f) {
			x /= f; y /= f; z /= f; return *this;
		}

		Vector3f & operator = (const Vector3f & v) {
			x = v.x; y = v.y; z = v.z; return *this;
		}
		Vector3f & operator += (const Vector3f & v) {
			x += v.x; y += v.y; z += v.z; return *this;
		}
		Vector3f & operator -= (const Vector3f & v) {
			x -= v.x; y -= v.y; z -= v.z; return *this;
		}
		Vector3f & operator *= (const Vector3f & v) {
			x *= v.x; y *= v.y; z *= v.z; return *this;
		}
		Vector3f & operator /= (const Vector3f & v) {
			x /= v.x; y /= v.y; z /= v.z; return *this;
		}

		void print_info() const {
			printf("x = %f, y = %f, z = %f\n", x, y, z);
		}

	};

#pragma endregion

#pragma region Box

	class Box {
	private:
		Vector3f min_vertex, max_vertex;
	public:
		Box() {}
		Box(const Box & box) :min_vertex(box.MinVertex()), max_vertex(box.MaxVertex()) {}
		Box(const Vector3f & min_vertex_, const Vector3f & max_vertex_) :min_vertex(min_vertex_), max_vertex(max_vertex_) {}

		Vector3f MaxVertex() const { return max_vertex; }
		Vector3f MinVertex() const { return min_vertex; }
	};

#pragma endregion

#pragma region Matrix4x4

	class Matrix4x4f {
	private:
		// 默认按行存储
		float data[4 * 4] = { 0.0 };
	public:
		Matrix4x4f();
		Matrix4x4f(const Matrix4x4f & mtrx);
		Matrix4x4f(const Matrix4x4f * mtrx);
		Matrix4x4f(const float(&data_)[16]);
		~Matrix4x4f();

		float GetData(int i) const;
		Float16 GetData() const;
		void SetData(int i, float f);

		bool operator == (const Matrix4x4f & mtrx);
		Matrix4x4f & operator = (const Matrix4x4f & mtrx);
		Matrix4x4f & operator += (const Matrix4x4f & mtrx);
		Matrix4x4f & operator -= (const Matrix4x4f & mtrx);
		Matrix4x4f & operator *= (const Matrix4x4f & mtrx);

		void print_info() const;
	};

#pragma endregion

#pragma region OverloadOperators

	inline Vector3f operator + (const Vector3f & v1, const Vector3f & v2) {
		return Vector3f(v1.X() + v2.X(), v1.Y() + v2.Y(), v1.Z() + v2.Z());
	}
	inline Vector3f operator - (const Vector3f & v1, const Vector3f & v2) {
		return Vector3f(v1.X() - v2.X(), v1.Y() - v2.Y(), v1.Z() - v2.Z());
	}
	inline Vector3f operator * (const Vector3f & v, float f) {
		return Vector3f(v.X() * f, v.Y() * f, v.Z() * f);
	}
	inline Vector3f operator * (float f, const Vector3f & v) {
		return Vector3f(v.X() * f, v.Y() * f, v.Z() * f);
	}
	inline Vector3f operator / (const Vector3f & v, float f) {
		return Vector3f(v.X() / f, v.Y() / f, v.Z() / f);
	}
	inline Vector3f operator / (const Vector3f & v1, const Vector3f & v2) {
		return Vector3f(v1.X() / v2.X(), v1.Y() / v2.Y(), v1.Z() / v2.Z());
	}

	inline Matrix4x4f operator + (Matrix4x4f mtrx1, const Matrix4x4f & mtrx2) {
		mtrx1 += mtrx2; return mtrx1;
	}

	inline Matrix4x4f operator - (Matrix4x4f mtrx1, const Matrix4x4f & mtrx2) {
		mtrx1 -= mtrx2; return mtrx1;
	}

	inline Matrix4x4f operator * (Matrix4x4f mtrx1, const Matrix4x4f & mtrx2) {
		mtrx1 *= mtrx2; return mtrx1;
	}

	inline Float4 operator * (const Matrix4x4f & mtrx, Float4 v4) {

		Float4 old_v4 = v4;
		Float16 data = mtrx.GetData();

		v4.x = 0.0;
		v4.x += data[0] * old_v4.x;
		v4.x += data[1] * old_v4.y;
		v4.x += data[2] * old_v4.z;
		v4.x += data[3] * old_v4.w;

		v4.y = 0.0;
		v4.y += data[4] * old_v4.x;
		v4.y += data[5] * old_v4.y;
		v4.y += data[6] * old_v4.z;
		v4.y += data[7] * old_v4.w;

		v4.z = 0.0;
		v4.z += data[8] * old_v4.x;
		v4.z += data[9] * old_v4.y;
		v4.z += data[10] * old_v4.z;
		v4.z += data[11] * old_v4.w;

		v4.w = 0.0;
		v4.w += data[12] * old_v4.x;
		v4.w += data[13] * old_v4.y;
		v4.w += data[14] * old_v4.z;
		v4.w += data[15] * old_v4.w;

		return v4;
	}
	inline Float4 operator * (Float4 v4, const Matrix4x4f & mtrx) {

		Float4 old_v4 = v4;
		Float16 data = mtrx.GetData();

		v4.x = 0.0;
		v4.x += old_v4.x * data[0];
		v4.x += old_v4.y * data[4];
		v4.x += old_v4.z * data[8];
		v4.x += old_v4.w * data[12];

		v4.y = 0.0;
		v4.y += old_v4.x * data[1];
		v4.y += old_v4.y * data[5];
		v4.y += old_v4.z * data[9];
		v4.y += old_v4.w * data[13];

		v4.z = 0.0;
		v4.z += old_v4.x * data[2];
		v4.z += old_v4.y * data[6];
		v4.z += old_v4.z * data[10];
		v4.z += old_v4.w * data[14];

		v4.w = 0.0;
		v4.w += old_v4.x * data[3];
		v4.w += old_v4.y * data[7];
		v4.w += old_v4.z * data[11];
		v4.w += old_v4.w * data[15];

		return v4;
	}


#pragma endregion

#pragma region Typedef

	typedef Matrix4x4f Mtx4f;

#pragma endregion

}



#endif