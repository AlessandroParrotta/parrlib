#pragma once

//#include "util.h"
#include "otherutil.h"
#include "Vector3f.h"
#include "Matrix4f.h"

#include <sstream>
#include <string>

namespace prb {
	class Quaternion
	{
	public:
		float x, y, z, w;

		Quaternion operator* (const Quaternion& q) const;
		//Quaternion operator* (const Vector3f &v);
		Vector3f operator* (const Vector3f& v) const;

		Quaternion operator+ (const Quaternion& q) const;
		Quaternion& operator*= (const Quaternion& q);

		Quaternion operator/(float f) const;

		mat4 operator*(mat4 const& m) const;

		Quaternion inverted() const;
		Quaternion scale(float s) const;

		float getTheta(float angle) const;
		float magnitude() const;

		Quaternion normalized() const;
		Quaternion conjugate() const;

		Vector3f direction() const;

		Matrix4f toMatrix() const;

		Quaternion fromEuler(vec3 const& euler) const;
		Vector3f toEuler() const;

		Quaternion lerp(Quaternion const& b, float t) const;
		Quaternion slerp(Quaternion const& b, float t) const;

		void setX(float angle);
		void setY(float angle);
		void setZ(float angle);

		float getX() const;
		float getY() const;
		float getZ() const;

		float getAngle() const;
		Vector3f getAxis() const;

		vec3 fw() const;
		vec3 ri() const;
		vec3 up() const;

		std::string toStringRadians() const;
		std::string toString() const;

		Quaternion();
		Quaternion(vec3 const& v);
		Quaternion(float angle, vec3 const& axis);
		Quaternion(float x, float y, float z, float w);
		Quaternion(float f);

		~Quaternion();
	};

	mat4 operator*(mat4 const& m, Quaternion const& q);

	std::ostream& operator<<(std::ostream& os, Quaternion const& q);
	std::wostream& operator<<(std::wostream& os, Quaternion const& q);

	typedef Quaternion quat;

	namespace pquat {
		const quat iden = { 0.f,0.f,0.f,1.f };	//identity quaternion

		Quaternion fromMatrix(Matrix4f mat);
		Quaternion lookAt(vec3 const& eye, vec3 const& target, vec3 const& up);
	}


	//a quaternion can also be interpreted as the rotation part of a matrix
}
