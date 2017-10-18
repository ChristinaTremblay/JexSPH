
#include "JexMath.h"

namespace Jex {

	Float3::Float3() { x = y = z = 0.0; }
	Float3::Float3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
	Float3::Float3(const Vector3f v) { x = v.X(); y = v.Y(); z = v.Z(); }
	Float3::Float3(const Vector3f & v) { x = v.X(); y = v.Y(); z = v.Z(); }
	Float3::Float3(const Vector3f * v) { x = v->X(); y = v->Y(); z = v->Z(); }

}