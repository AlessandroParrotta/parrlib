#pragma once

#include "util.h"
#include "Vector3f.h"
#include "Matrix4f.h"

#include <sstream>
#include <string>

class Quaternion
{
public:
	float x, y, z, w;

	Quaternion operator* (const Quaternion &q) const;
	//Quaternion operator* (const Vector3f &v);
	Vector3f operator* (const Vector3f &v) const;

	Quaternion operator+ (const Quaternion &q) const;
	Quaternion& operator*= (const Quaternion &q);

	mat4 operator*(mat4 const& m) const;

	Quaternion inverse() const;
	Quaternion scale(float s) const;

	float getTheta(float angle) const;
	float magnitude() const;

	Quaternion normalized() const;
	Quaternion conjugate() const;

	Vector3f direction() const;

	Quaternion fromMatrix(Matrix4f mat) const;
	Matrix4f toMatrix() const;

	Quaternion Quaternion::createFromAxisAngle(Vector3f axis, float angle) const;
	Quaternion Quaternion::lookAt(Vector3f sourcePoint, Vector3f destPoint, Vector3f up) const;

	Quaternion fromEuler(float x, float y, float z) const;
	Vector3f toEuler() const;

	void setX(float angle);
	void setY(float angle);
	void setZ(float angle);

	float getX() const;
	float getY() const;
	float getZ() const;

	float getAngle() const;
	Vector3f getAxis() const;

	std::string toStringRadians() const;
	std::string toString() const;

	Quaternion();
	Quaternion(vec3 const& v);
	Quaternion(float angle, vec3 const& axis);
	Quaternion(float x, float y, float z, float w);

	~Quaternion();
};

mat4 operator*(mat4 const& m, Quaternion const& q);

std::ostream& operator<<(std::ostream& os, Quaternion const& q);
std::wostream& operator<<(std::wostream& os, Quaternion const& q);

typedef Quaternion quat;


//a quaternion can also be interpreted as the rotation part of a matrix
