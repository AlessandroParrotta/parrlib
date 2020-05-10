#pragma once

#include <string>
#include <sstream>

#include "Vector2f.h"

class Vector4f;

class Vector3f {
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3f operator+ (const Vector3f &other) const;
	Vector3f operator- (const Vector3f &other) const;
	Vector3f operator* (const Vector3f &other) const;
	Vector3f operator/ (const Vector3f &other) const;

	Vector3f operator+= (const Vector3f &other);
	Vector3f operator-= (const Vector3f &other);
	Vector3f operator*= (const Vector3f &other);
	Vector3f operator/= (const Vector3f &other);

	bool operator> (const Vector3f &other) const;
	bool operator< (const Vector3f &other) const;
	bool operator>= (const Vector3f &other) const;
	bool operator<= (const Vector3f &other) const;
	bool operator== (const Vector3f &other) const;
	bool operator!= (const Vector3f &other) const;

	float& get(int i);
	float get(int i) const;
	float& operator[](int i);
	float operator[](int i) const;

	constexpr unsigned int length() const { return 3; } //returns 3 (number of components)

	Vector3f rotateX(float angle) const;
	Vector3f rotateY(float angle) const;
	Vector3f rotateZ(float angle) const;
	Vector3f rotate(float x, float y, float z) const;
	Vector3f rotate(Vector3f r) const;

	float magnitude() const;
	float dot(Vector3f other) const;
	Vector3f cross(Vector3f other) const;
	Vector3f reflect(Vector3f normal) const;
	Vector3f normalized() const;
	Vector3f abs() const;
	Vector3f clone() const;
	std::string toString() const;
	std::wstring toWString() const;

	Vector3f clamped(Vector3f vmin, Vector3f vmax);
	Vector3f floored();
	Vector3f ceiled();

	Vector2f xy() const;
	Vector2f yx() const;
	Vector2f yz() const;
	Vector2f zy() const;
	Vector2f xz() const;
	Vector2f zx() const;

	Vector3f nx() const;
	Vector3f ny() const;
	Vector3f nz() const;

	Vector3f();
	Vector3f(float x, float y, float z);
	Vector3f(float f);
	Vector3f(double d);
	Vector3f(float *f);

	Vector3f(Vector2f v2, float z);
	Vector3f(float x, Vector2f v2);
	Vector3f(Vector4f v);

	//colors
	Vector3f(std::string hex);
	Vector3f(const char *hex);
	Vector3f(unsigned int hex);

	~Vector3f();
};

Vector3f operator+ (const float& f, const Vector3f& v);
Vector3f operator- (const float& f, const Vector3f& v);
Vector3f operator* (const float& f, const Vector3f& v);
Vector3f operator/ (const float& f, const Vector3f& v);

Vector3f operator- (Vector3f v);

std::ostream &operator<<(std::ostream &os, Vector3f const& v);
std::wostream &operator<<(std::wostream &os, Vector3f const& v);

namespace vColor3f {
	extern Vector3f white;
	extern Vector3f black;
	extern Vector3f red;
	extern Vector3f blue;
	extern Vector3f green;
	extern Vector3f cyan;
	extern Vector3f orange;
	extern Vector3f purple;
	extern Vector3f magenta;
	extern Vector3f gray;

	Vector3f rand();
}

typedef Vector3f vec3;
namespace vc3 = vColor3f;

vec3 vec3x(float x);
vec3 vec3y(float y);
vec3 vec3z(float z);

vec3 vec3xy(float x, float y); vec3 vec3xy(vec2 xy);
vec3 vec3yz(float y, float z); vec3 vec3yz(vec2 yz);
vec3 vec3xz(float x, float z); vec3 vec3xz(vec2 xz);

vec3 vec3xff(float x, float f); vec3 vec3xz(vec2 fx);
vec3 vec3fyf(float y, float f); vec3 vec3xz(vec2 fy);
vec3 vec3ffz(float z, float f); vec3 vec3xz(vec2 fz);

vec3 vec3rand();

namespace v3ax {
	const vec3 upv3 = { 0.f, 1.f, 0.f };
	const vec3 riv3 = { 1.f, 0.f, 0.f };
	const vec3 fwv3 = { 0.f, 0.f, 1.f };
}
