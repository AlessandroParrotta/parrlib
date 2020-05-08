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

	Quaternion operator* (const Quaternion &q);
	//Quaternion operator* (const Vector3f &v);

	Vector3f operator* (const Vector3f &v);

	Quaternion operator+ (const Quaternion &q);

	Quaternion operator*= (const Quaternion &q);

	Quaternion inverse();

	Quaternion scale(float s);

	float getTheta(float angle);
	float magnitude();

	Quaternion normalized();
	Quaternion conjugate();

	Vector3f direction();

	Quaternion fromMatrix(Matrix4f mat);
	Matrix4f toMatrix();

	Quaternion Quaternion::createFromAxisAngle(Vector3f axis, float angle);
	Quaternion Quaternion::lookAt(Vector3f sourcePoint, Vector3f destPoint, Vector3f up);

	Quaternion fromEuler(float x, float y, float z);
	Vector3f toEuler();

	void setX(float angle);
	void setY(float angle);
	void setZ(float angle);

	float getX();
	float getY();
	float getZ();

	float getAngle();
	Vector3f getAxis();

	std::string toStringRadians();
	std::string toString();

	Quaternion();
	Quaternion(Vector3f v);
	Quaternion(Vector3f axis, float angle);
	Quaternion(float x, float y, float z);
	Quaternion(float x, float y, float z, float w);

	~Quaternion();
};

