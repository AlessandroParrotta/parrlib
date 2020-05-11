#include <parrlib/Vector2f.h>
#include <math.h>
#include <iostream>
#include <sstream>

#include <parrlib/Vector3f.h>
#include <parrlib/stringutils.h>
#include <parrlib/util.h>

Vector2f Vector2f::operator+ (const Vector2f &other) const {
	return Vector2f(x + other.x, y + other.y);
}

Vector2f Vector2f::operator- (const Vector2f &other) const {
	return Vector2f(x - other.x, y - other.y);
}

Vector2f Vector2f::operator* (const Vector2f &other) const {
	return Vector2f(x * other.x, y * other.y);
}

Vector2f Vector2f::operator/ (const Vector2f &other) const {
	return Vector2f(x / other.x, y / other.y);
}


Vector2f operator+ (float f, Vector2f v) {
	return Vector2f(f + v.x, f + v.y);
}

Vector2f operator- (float f, Vector2f v) {
	return Vector2f(f - v.x, f - v.y);
}

Vector2f operator* (float f, Vector2f v) {
	return Vector2f(f * v.x, f * v.y);
}

Vector2f operator/ (float f, Vector2f v) {
	return Vector2f(f / v.x, f / v.y);
}


Vector2f operator- (Vector2f v) {
	return Vector2f(-v.x, -v.y);
}


Vector2f Vector2f::operator+= (const Vector2f &other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2f Vector2f::operator-= (const Vector2f &other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2f Vector2f::operator*= (const Vector2f &other) {
	x *= other.x;
	y *= other.y;
	return *this;
}

Vector2f Vector2f::operator/= (const Vector2f &other) {
	x /= other.x;
	y /= other.y;
	return *this;
}


bool Vector2f::operator> (const Vector2f &other) const {
	return this->x > other.x && this->y > other.y;
}

bool Vector2f::operator< (const Vector2f &other) const {
	return this->x < other.x && this->y < other.y;
}

bool Vector2f::operator>= (const Vector2f &other) const {
	return this->x >= other.x && this->y >= other.y;
}

bool Vector2f::operator<= (const Vector2f &other) const {
	return this->x <= other.x && this->y <= other.y;
}

bool Vector2f::operator== (const Vector2f &other) const {
	return this->x == other.x && this->y == other.y;
}

bool Vector2f::operator!= (const Vector2f &other) const {
	return !(*this == other);
}

bool Vector2f::equal(Vector2f const& v) const { return *this == v; }
bool Vector2f::equal(Vector2f const& v, float bias) const {
	return	x > v.x - bias && x < v.x + bias &&
			y > v.y - bias && y < v.y + bias;
}

float& Vector2f::get(int i) {
	switch (i) {
		case 0:return x;
		case 1:return y;
	}
}
float Vector2f::get(int i) const {
	switch (i) {
		case 0:return x;
		case 1:return y;
	}
	return 0.f;
}

float& Vector2f::operator[](int i) { return get(i); }
float Vector2f::operator[](int i) const{ return get(i); }

std::ostream &operator<<(std::ostream &os, Vector2f const& v) {
	return os << v.toString();
}

std::wostream& operator<<(std::wostream& os, Vector2f const& v) {
	return os << v.toWString();
}


float Vector2f::magnitude() const {
	return sqrt((x*x) + (y*y));	
}

float Vector2f::dot(Vector2f other) const {
	return x * other.x + y * other.y;
}

Vector2f Vector2f::normalized() const {
	float magnitude2 = magnitude();
	return Vector2f(x/magnitude2, y/magnitude2);
}

float Vector2f::cross(Vector2f v) const {
	return x * v.y - y * v.x;
}

Vector2f Vector2f::perp() const {
	return Vector2f(-y, x);
}

Vector2f Vector2f::abs() const {
	float aX = std::abs(x);
	float aY = std::abs(y);
	return Vector2f(aX, aY);
}

Vector2f Vector2f::minned(Vector2f val) const { return Vector2f(std::fmin(x, val.x), std::fmin(y, val.y)); }
Vector2f Vector2f::minned(float val) const { return minned(Vector2f(val)); }

Vector2f Vector2f::maxed(Vector2f val) const { return Vector2f(std::fmax(x, val.x), std::fmax(y, val.y)); }
Vector2f Vector2f::maxed(float val) const { return maxed(Vector2f(val)); }

Vector2f Vector2f::floored() const { return { std::floorf(x), std::floorf(y) }; }
Vector2f Vector2f::ceiled() const { return { std::ceilf(x), std::ceilf(y) }; }

Vector2f Vector2f::clamped(float min, float max) const {
	return Vector2f(
		std::fmin(std::fmax(x, min), max),
		std::fmin(std::fmax(y, min), max)
	);
}

Vector2f Vector2f::clamped(Vector2f rangex, Vector2f rangey) const {
	return Vector2f(
		std::fmin(std::fmax(x, rangex.x), rangex.y),
		std::fmin(std::fmax(y, rangey.x), rangey.y)
	);
}

Vector2f Vector2f::clamped(Vector2f v) const { 
	return Vector2f(
			std::fmin(std::fmax(x, v.x), v.y),
			std::fmin(std::fmax(y, v.x), v.y)
	); 
}

float Vector2f::aspectx() const { return x / y; }
float Vector2f::aspecty() const { return y / x; }
float Vector2f::aspectmax() const { float ax = aspectx(), ay = aspecty(); return std::fmax(ax, ay); }
float Vector2f::aspectmin() const { float ax = aspectx(), ay = aspecty(); return std::fmin(ax, ay); }
Vector2f Vector2f::aspectmaxv() const { float ax = aspectx(), ay = aspecty(); return std::fabs(ax) > std::fabs(ay) ? vec2(ax, 1.f) : vec2(1.f, ay); }
Vector2f Vector2f::aspectminv() const { float ax = aspectx(), ay = aspecty(); return std::fabs(ax) < std::fabs(ay) ? vec2(ax, 1.f) : vec2(1.f, ay); }
Vector2f Vector2f::aspectxy() const { return Vector2f(aspectx(), aspecty()); }

float Vector2f::maxv() const { return std::fmax(x, y); }
float Vector2f::minv() const { return std::fmin(x, y); }

Vector2f Vector2f::invy(float ref) const { return Vector2f(x, ref-y); }
Vector2f Vector2f::invx(float ref) const { return Vector2f(ref-x, y); }
Vector2f Vector2f::inv(Vector2f ref) const { return Vector2f(ref.x - x, ref.y - y); }

Vector2f Vector2f::rotate(Vector2f pivot, float angle, float ortho) const {
	float theta = angle * 3.141592653589793238463f / 180.0f;
	return Vector2f(pivot.x*ortho + (cos(theta) * (x*ortho - pivot.x*ortho) - sin(theta) * (y - pivot.y)),
		pivot.y + (sin(theta) * (x*ortho - pivot.x*ortho) + cos(theta) * (y - pivot.y)));
}

Vector2f Vector2f::rotate(Vector2f pivot, float angle) const {
	float theta = angle* 3.141592653589793238463f /180.0f;
	return Vector2f(pivot.x+(cos(theta) * (x - pivot.x) - sin(theta) * (y - pivot.y) ),
					pivot.y+(sin(theta) * (x - pivot.x) + cos(theta) * (y - pivot.y) ));
}

Vector2f Vector2f::rotate(float angle)  const {
	return rotate(0.0f, angle);
}

bool Vector2f::equalsRange(Vector2f v, Vector2f range) const {
	return std::abs(x-v.x) >= -range.x && std::abs(x-v.x) <= range.x &&
			std::abs(y - v.y) >= -range.y && std::abs(y - v.y) <= range.y;
}

bool Vector2f::equalsRange(Vector2f v, float range)  const {
	return equalsRange(v, Vector2f(range));
}

Vector2f Vector2f::getClone() const {
	return Vector2f(x, y);
}

std::string Vector2f::toString() const {
	//std::ostringstream ss;
	//ss << "(" << x << ", " << y << ")";
	//return ss.str();
	std::stringstream ss;
	ss << "(" << x << ", " << y << ")";
	return ss.str();
}

std::wstring Vector2f::toWString() const { return stru::toWString(toString()); }

Vector2f::Vector2f(){

}

Vector2f::Vector2f(float xy) {
	this->x = this->y = xy;
}

Vector2f::Vector2f(float x, float y) {
	this->x = x;
	this->y = y;
}

//Vector2f::Vector2f(int x, int y) {
//	this->x = (float)x;
//	this->y = (float)y;
//}
//
//Vector2f::Vector2f(unsigned int x, unsigned int y) {
//	this->x = (float)x;
//	this->y = (float)y;
//}
//
//Vector2f::Vector2f(double x, double y) {
//	this->x = (float)x;
//	this->y = (float)y;
//}
//
//Vector2f::Vector2f(char x, char y) {
//	this->x = (float)x;
//	this->y = (float)y;
//}

Vector2f::Vector2f(Vector3f v) {
	x = v.x;
	y = v.y;
}


Vector2f::~Vector2f(){

}

Vector2f Vector2f::yx() const { return { y, x }; }

Vector2f Vector2f::xx() const { return { x, x }; }
Vector2f Vector2f::yy() const { return { y, y }; }

Vector2f Vector2f::nx() const { return { -x, y }; }
Vector2f Vector2f::ny() const { return { x, -y }; }

Vector2f Vector2f::xo() const { return { x, 0.f }; }
Vector2f Vector2f::oy() const { return { 0.f, y }; }
Vector2f Vector2f::yo() const { return { y, 0.f }; }
Vector2f Vector2f::ox() const { return { 0.f, x }; }

vec2 vec2x(float x) { return vec2(x, 0.f); }
vec2 vec2y(float y) { return vec2(0.f, y); }

vec2 vec2rand() { return { util::frand(), util::frand() }; }

namespace v2u {
	vec2 x(float x) { return { x, 0.f }; }
	vec2 y(float y) { return { 0.f, y }; }
}
