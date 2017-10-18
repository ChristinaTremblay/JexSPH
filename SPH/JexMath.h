#pragma once

#ifndef JEX_MATH_H
#define JEX_MATH_H

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
			return x * v.x + y * v.y + z * v.z;
		}

		float LengthSq() const { return x*x + y*y + z*z; }
		float Length() const { return sqrt(LengthSq()); }

		void Normalize() {
			if (LengthSq() > EPSILON) {
				float len = Length();
				x /= len; y /= len; z /= len;
			}
		}

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

#pragma region InlineFunctions

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



}

#endif