#include <parrlib/Vector3f.h>

#include <parrlib/Vector4f.h>
#include <parrlib/stringutils.h>

#include <parrlib/OtherUtil.h>

Vector3f Vector3f::operator+ (const Vector3f &other) const {
	return Vector3f(x + other.x, y + other.y, z + other.z);
}

Vector3f Vector3f::operator- (const Vector3f &other) const {
	return Vector3f(x - other.x, y - other.y, z - other.z);
}

Vector3f Vector3f::operator* (const Vector3f &other) const {
	return Vector3f(x * other.x, y * other.y, z * other.z);
}

Vector3f Vector3f::operator/ (const Vector3f &other) const {
	return Vector3f(x / other.x, y / other.y, z / other.z);
}


Vector3f operator+ (const float &f, const Vector3f &v) {
	return Vector3f(f + v.x, f + v.y, f + v.z);
}

Vector3f operator- (const float& f, const Vector3f& v) {
	return Vector3f(f - v.x, f - v.y, f - v.z);
}

Vector3f operator* (const float& f, const Vector3f& v) {
	return Vector3f(f * v.x, f * v.y, f * v.z);
}

Vector3f operator/ (const float& f, const Vector3f& v) {
	return Vector3f(f / v.x, f / v.y, f / v.z);
}


Vector3f operator- (Vector3f v) {
	return Vector3f(-v.x, -v.y, -v.z);
}


Vector3f Vector3f::operator+= (const Vector3f &other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3f Vector3f::operator-= (const Vector3f &other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3f Vector3f::operator*= (const Vector3f &other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

Vector3f Vector3f::operator/= (const Vector3f &other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}


bool Vector3f::operator> (const Vector3f &other) const {
	return this->x > other.x && this->y > other.y && this->z > other.z;
}

bool Vector3f::operator< (const Vector3f &other) const {
	return this->x < other.x && this->y < other.y && this->z < other.z;
}

bool Vector3f::operator>= (const Vector3f &other) const {
	return this->x >= other.x && this->y >= other.y && this->z >= other.z;
}

bool Vector3f::operator<= (const Vector3f &other) const {
	return this->x <= other.x && this->y <= other.y && this->z <= other.z;
}

bool Vector3f::operator== (const Vector3f &other) const {
	return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool Vector3f::operator!= (const Vector3f &other) const {
	return !(*this == other);
}

float& Vector3f::get(int i) {
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	}
}
float Vector3f::get(int i) const { 
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	}
}

float& Vector3f::operator[](int i) { return get(i); }
float Vector3f::operator[](int i) const { return get(i); }

std::ostream &operator<<(std::ostream &os, Vector3f const& v) {
	return os << v.toString();
}

std::wostream& operator<<(std::wostream& os, Vector3f const& v) {
	return os << v.toWString();
}

Vector3f Vector3f::rotateX(float angle) const {
	angle = (float)outl::toRadians(angle);

	float x = this->x;
	float y = this->y * cos(angle) - this->z * sin(angle);
	float z = this->y * sin(angle) + this->z * cos(angle);

	return Vector3f(x, y, z);
}

Vector3f Vector3f::rotateY(float angle) const {
	angle = (float)outl::toRadians(angle);

	float x = this->x * cos(angle) + this->z * sin(angle);
	float y = this->y;
	float z = -this->x * sin(angle) + this->z * cos(angle);

	return Vector3f(x, y, z);
}

Vector3f Vector3f::rotateZ(float angle) const {
	angle = (float)outl::toRadians(angle);

	float x = this->x * cos(angle) - this->y * sin(angle);
	float y = this->x * sin(angle) + this->y * cos(angle);
	float z = this->z;

	return Vector3f(x, y, z);
}

Vector3f Vector3f::rotate(float x, float y, float z) const {
	return clone().rotateX(x).rotateY(y).rotateZ(z);
}

Vector3f Vector3f::rotate(Vector3f r) const {
	return rotate(r.x, r.y, r.z);
}


float Vector3f::dot(Vector3f other) const {
	return x * other.x + y * other.y + z * other.z;
}

Vector3f Vector3f::cross(Vector3f other) const {
	return Vector3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

Vector3f Vector3f::reflect(Vector3f normal) const {
	return *this - 2.0f*normal.dot(*this)*normal;
}

float Vector3f::magnitude() const {
	return sqrt((x*x) + (y*y) + (z*z));
}

Vector3f Vector3f::normalized() const {
	float magnitude2 = magnitude();
	return Vector3f(x / magnitude2, y / magnitude2, z / magnitude2);
}

Vector3f Vector3f::abs() const {
	float aX = std::abs(x);
	float aY = std::abs(y);
	float aZ = std::abs(z);
	return Vector3f(aX, aY, aZ);
}

Vector3f Vector3f::clone() const {
	return Vector3f(x, y, z);
}

std::string Vector3f::toString() const {
	std::stringstream ss;
	ss << "(" << x << ", " << y << ", " << z << ")";
	return ss.str();
}

std::wstring Vector3f::toWString() const {
	std::wstringstream ss;
	ss << "(" << x << ", " << y << ", " << z << ")";
	return ss.str();
}

Vector3f Vector3f::clamped(Vector3f vmin, Vector3f vmax) {
	return { 
		std::fmin(std::fmax(x, vmin.x), vmax.x),
		std::fmin(std::fmax(y, vmin.y), vmax.y),
		std::fmin(std::fmax(z, vmin.z), vmax.z),
	};
}

Vector3f Vector3f::floored() { return { std::floorf(x),std::floorf(y),std::floorf(z), }; }
Vector3f Vector3f::ceiled() { return { std::ceilf(x),std::ceilf(y),std::ceilf(z), }; }


Vector3f::Vector3f() {

}

Vector3f::Vector3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3f::Vector3f(float f) {
	this->x = f;
	this->y = f;
	this->z = f;
}

Vector3f::Vector3f(double d) : Vector3f((float)d){}

Vector3f::Vector3f(float *f) {
	x = f[0];
	y = f[1];
	z = f[2];
}

Vector3f::Vector3f(Vector2f v2, float z) {
	this->x = v2.x;
	this->y = v2.y;
	this->z = z;
}

Vector3f::Vector3f(float x, Vector2f v2) {
	this->x = x;
	this->y = v2.x;
	this->z = v2.y;
}

Vector3f::Vector3f(Vector4f v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3f::Vector3f(std::string hex) {
	hex = hex.substr(1, hex.length() - 1);
	for (int i = hex.length(); i < 6; i++) hex += "0"; //adapt to standard size, 2*4 values

	x = (float)((stringutils::getHexVal(hex[1]) * 16) + stringutils::getHexVal(hex[0])) / 255.f;
	y = (float)((stringutils::getHexVal(hex[3]) * 16) + stringutils::getHexVal(hex[2])) / 255.f;
	z = (float)((stringutils::getHexVal(hex[5]) * 16) + stringutils::getHexVal(hex[4])) / 255.f;
}

Vector3f::Vector3f(const char* hex) : Vector3f(std::string(hex)) {}

Vector3f::Vector3f(unsigned int hex) {
	unsigned char b3 = hex & 0x000000ff;
	unsigned char b2 = (hex & 0x0000ff00) >> 8;
	unsigned char b1 = (hex & 0x00ff0000) >> 16;
	//unsigned char b0 = (hex & 0xff000000) >> 24;

	x = (float)b1 / 255.f;
	y = (float)b2 / 255.f;
	z = (float)b3 / 255.f;
	//w = (float)b3 / 255.f;
}


Vector3f::~Vector3f() {

}

Vector2f Vector3f::xy() const {
	return Vector2f(x, y);
}

Vector2f Vector3f::yx() const {
	return Vector2f(y, x);
}

Vector2f Vector3f::yz() const {
	return Vector2f(y, z);
}

Vector2f Vector3f::zy() const {
	return Vector2f(z, y);
}

Vector2f Vector3f::xz() const {
	return Vector2f(x, z);
}

Vector2f Vector3f::zx() const {
	return Vector2f(z, x);
}

Vector3f Vector3f::nx() const { return { -x,y,z }; }
Vector3f Vector3f::ny() const { return { x,-y,z }; }
Vector3f Vector3f::nz() const { return { x,y,-z }; }

namespace vColor3f {
	Vector3f white(1.0f);
	Vector3f black(0.0f, 0.0f, 0.0f);
	Vector3f red(1.0f, 0.0f, 0.0f);
	Vector3f blue(0.0f, 0.0f, 1.0f);
	Vector3f green(0.0f, 1.0f, 0.0f);
	Vector3f cyan(0.0f, 1.0f, 1.0f);
	Vector3f orange(1.0f, 0.64453125f, 0.0f);
	Vector3f purple(0.5f, 0.0f, 0.5f);
	Vector3f magenta(1.0f, 0.0f, 1.0f);
	Vector3f gray(0.5f, 0.5f, 0.5f);

	Vector3f rand() { return { outl::frand(),outl::frand(),outl::frand() }; }
}

vec3 vec3x(float x) { return vec3(x, 0.f, 0.f); }
vec3 vec3y(float y) { return vec3(0.f, y, 0.f); }
vec3 vec3z(float z) { return vec3(0.f, 0.f, z); }

vec3 vec3xy(float x, float y) { return vec3(x, y, 0.f); } vec3 vec3xy(vec2 xy) { return vec3(xy, 0.f); }
vec3 vec3yz(float y, float z) { return vec3(0.f, y, z); } vec3 vec3yz(vec2 yz) { return vec3(0.f, yz); }
vec3 vec3xz(float x, float z) { return vec3(x, 0.f, z); } vec3 vec3xz(vec2 xz) { return vec3(xz.x, 0.f, xz.y); }

vec3 vec3rand() { return { outl::frand(), outl::frand(), outl::frand() }; }
