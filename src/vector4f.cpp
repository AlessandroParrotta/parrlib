#include <parrlib/Vector4f.h>

#include <iostream>
#include <parrlib/stringutils.h>
#include <parrlib/util.h>

Vector4f Vector4f::right() {
	return Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
}

Vector4f Vector4f::up() {
	return Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
}

Vector4f Vector4f::forward() {
	return Vector4f(0.0f, 0.0f, 1.0f, 0.0f);
}


Vector4f Vector4f::operator+ (const Vector4f &other) const {
	return Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4f Vector4f::operator- (const Vector4f &other) const {
	return Vector4f(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4f Vector4f::operator* (const Vector4f &other) const {
	return Vector4f(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4f Vector4f::operator/ (const Vector4f &other) const {
	return Vector4f(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vector4f operator+ (float f, Vector4f const& v) {
	return Vector4f(f + v.x, f + v.y, f + v.z, f + v.w);
}

Vector4f operator- (float f, Vector4f const& v) {
	return Vector4f(f - v.x, f - v.y, f - v.z, f - v.w);
}

Vector4f operator* (float f, Vector4f const& v) {
	return Vector4f(f * v.x, f * v.y, f * v.z, f * v.w);
}

Vector4f operator/ (float f, Vector4f const& v) {
	return Vector4f(f / v.x, f / v.y, f / v.z, f / v.w);
}


Vector4f operator- (Vector4f const& v) {
	return Vector4f(-v.x, -v.y, -v.z, -v.w);
}


Vector4f Vector4f::operator+= (const Vector4f &other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vector4f Vector4f::operator-= (const Vector4f &other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vector4f Vector4f::operator*= (const Vector4f &other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

Vector4f Vector4f::operator/= (const Vector4f &other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
	return *this;
}


bool Vector4f::operator> (const Vector4f &other) const {
	return this->x > other.x && this->y > other.y && this->z > other.z && this->w > other.w;
}

bool Vector4f::operator< (const Vector4f &other) const {
	return this->x < other.x && this->y < other.y && this->z < other.z && this->w < other.w;
}

bool Vector4f::operator>= (const Vector4f &other) const {
	return this->x >= other.x && this->y >= other.y && this->z >= other.z && this->w >= other.w;
}

bool Vector4f::operator<= (const Vector4f &other) const {
	return this->x <= other.x && this->y <= other.y && this->z <= other.z && this->w <= other.w;
}

bool Vector4f::operator== (const Vector4f &other) const {
	return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
}

bool Vector4f::operator!= (const Vector4f &other) const {
	return !(*this == other);
}

float Vector4f::operator[] (int i) const {
	switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
	return 0.f;
}
float& Vector4f::operator[] (int i) {
	switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
	return x;
}

std::ostream &operator<<(std::ostream &os, Vector4f  const& v) {
	return os << v.toString();
}
std::wostream& operator<<(std::wostream& os, Vector4f const& v) { return os << stru::towstr(v.toString()); }


float Vector4f::dot(Vector4f other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

float Vector4f::magnitude() const {
	return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

Vector4f Vector4f::normalized() const {
	float magnitude2 = magnitude();
	return Vector4f(x / magnitude2, y / magnitude2, z / magnitude2, w / magnitude2);
}

Vector4f Vector4f::abs() const {
	float aX = std::abs(x);
	float aY = std::abs(y);
	float aZ = std::abs(z);
	float aW = std::abs(w);
	return Vector4f(aX, aY, aZ, aW);
}

Vector4f Vector4f::clone() const {
	return Vector4f(x, y, z, w);
}

std::string Vector4f::toString() const {
	std::stringstream ss;
	ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return ss.str();
}


Vector4f Vector4f::colMul(float xf, float yf, float zf) const {
	return Vector4f(x*xf, y*yf, z*zf, w);
}

Vector4f Vector4f::colMul(Vector3f f) const {
	return colMul(f.x, f.y, f.z);
}

Vector4f Vector4f::colMul(float f) const {
	return colMul(f,f,f);
}

Vector4f Vector4f::colDiv(float xf, float yf, float zf) const {
	return Vector4f(x / xf, y / yf, z / zf, w);
}

Vector4f Vector4f::colDiv(Vector3f f) const {
	return colDiv(f.x, f.y, f.z);
}

Vector4f Vector4f::colDiv(float f) const {
	return colDiv(f, f, f);
}

Vector4f Vector4f::realColMul(float factor) const {
	return *this + Vector4f( 1.f)*factor;
}

Vector4f Vector4f::invCol() const{
	return Vector4f(-x, -y, -z, 1.f);
}

Vector4f Vector4f::negCol() const {
	return Vector4f(1.f - x, 1.f - y, 1.f - z, 1.f);
}

Vector4f Vector4f::sz(std::string const& str) const {
	Vector4f res = 0.f;

	int curcomp = 0;
	int cur = 0;
	bool neg = false;
	while (cur < str.length() && curcomp < 4) {
		if (str[cur] == 'n') {
			neg = true;
		}
		else {
			if (str[cur] == 'x')		res[curcomp] = x * (neg ? -1.f : 1.f);
			else if (str[cur] == 'y')	res[curcomp] = y * (neg ? -1.f : 1.f);
			else if (str[cur] == 'z')	res[curcomp] = z * (neg ? -1.f : 1.f);
			else if (str[cur] == 'w')	res[curcomp] = w * (neg ? -1.f : 1.f);
			else if (str[cur] == 'o')	res[curcomp] = 0.f;
			neg = false;
			curcomp++;
		}
		cur++;
	}

	return res;
}

//constexpr Vector4f Vector4f::sz4(std::string const& str) const {
//	if (str.length() <= 4) return 0.f;
//	return sz(str);
//}
//constexpr Vector3f Vector4f::sz3(std::string const& str) const {
//	if (str.length() <= 3) return 0.f;
//	return sz(str).xyz();
//}
//constexpr Vector2f Vector4f::sz2(std::string const& str) const {
//	if (str.length() <= 2) return 0.f;
//	return sz(str).xy();
//}

Vector4f::Vector4f() {
	
}

Vector4f::Vector4f(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4f::Vector4f(Vector2f v2, float z, float w) {
	this->x = v2.x;
	this->y = v2.y;
	this->z = z;
	this->w = w;
}

Vector4f::Vector4f(float x, Vector2f v2, float w) {
	this->x = x;
	this->y = v2.x;
	this->z = v2.y;
	this->w = w;
}

Vector4f::Vector4f(float x, float y, Vector2f v2) {
	this->x = x;
	this->y = y;
	this->z = v2.x;
	this->w = v2.y;
}

Vector4f::Vector4f(Vector2f v1, Vector2f v2) {
	this->x = v1.x;
	this->y = v1.y;
	this->z = v2.x;
	this->w = v2.y;
}

Vector4f::Vector4f(Vector3f v3, float w) {
	this->x = v3.x;
	this->y = v3.y;
	this->z = v3.z;
	this->w = w;
}

Vector4f::Vector4f(float x, Vector3f v3) {
	this->x = x;
	this->y = v3.x;
	this->z = v3.y;
	this->w = v3.z;
}

Vector4f::Vector4f(float f) {
	this->x = f;
	this->y = f;
	this->z = f;
	this->w = f;
}

Vector4f::Vector4f(float *f) {
	x = f[0];
	y = f[1];
	z = f[2];
	w = f[3];
}

Vector4f::Vector4f(std::string hex) {
	hex = hex.substr(1, hex.length()-1);
	for (int i = hex.length(); i < 8; i++) hex += "0"; //adapt to standard size, 2*4 values

	x = (float)((stru::getHexVal(hex[0])*16) + stru::getHexVal(hex[1]))/255.f;
	y = (float)((stru::getHexVal(hex[2])*16) + stru::getHexVal(hex[3]))/255.f;
	z = (float)((stru::getHexVal(hex[4])*16) + stru::getHexVal(hex[5]))/255.f;
	w = (float)((stru::getHexVal(hex[6])*16) + stru::getHexVal(hex[7]))/255.f;
}

Vector4f::Vector4f(const char* hex) : Vector4f(std::string(hex)) {}

Vector4f::Vector4f(unsigned int hex) {
	unsigned char b3 = hex & 0x000000ff;
	unsigned char b2 = (hex & 0x0000ff00) >> 8;
	unsigned char b1 = (hex & 0x00ff0000) >> 16;
	unsigned char b0 = (hex & 0xff000000) >> 24;

	x = (float)b0 / 255.f;
	y = (float)b1 / 255.f;
	z = (float)b2 / 255.f;
	w = (float)b3 / 255.f;
}


Vector4f::~Vector4f(){

}

Vector2f Vector4f::xy() const {
	return Vector2f(x, y);
}
Vector2f Vector4f::xz() const {
	return Vector2f(x, z);
}
Vector2f Vector4f::xw() const {
	return Vector2f(x, w);
}

Vector2f Vector4f::yx() const {
	return Vector2f(y, x);
}
Vector2f Vector4f::yz() const {
	return Vector2f(y, z);
}
Vector2f Vector4f::yw() const {
	return Vector2f(y, w);
}

Vector2f Vector4f::zx() const {
	return Vector2f(z, x);
}
Vector2f Vector4f::zy() const {
	return Vector2f(z, y);
}
Vector2f Vector4f::zw() const {
	return Vector2f(z, w);
}

Vector2f Vector4f::wx() const {
	return Vector2f(w, x);
}
Vector2f Vector4f::wy() const {
	return Vector2f(w, y);
}
Vector2f Vector4f::wz() const {
	return Vector2f(w, z);
}

Vector3f Vector4f::xyz() const {
	return Vector3f(x,y,z);
}
Vector3f Vector4f::xzy() const {
	return Vector3f(x,z,y);
}
Vector3f Vector4f::xyw() const {
	return Vector3f(x,y,w);
}
Vector3f Vector4f::xwy() const {
	return Vector3f(x,w,y);
}
Vector3f Vector4f::xzw() const {
	return Vector3f(x,z,w);
}
Vector3f Vector4f::xwz() const {
	return Vector3f(x,w,z);
}

Vector3f Vector4f::yxz() const {
	return Vector3f(y,x,z);
}
Vector3f Vector4f::yzx() const {
	return Vector3f(y, z, x);
}
Vector3f Vector4f::yxw() const {
	return Vector3f(y, x, w);
}
Vector3f Vector4f::ywx() const {
	return Vector3f(y, w, x);
}
Vector3f Vector4f::yzw() const {
	return Vector3f(y, z, w);
}
Vector3f Vector4f::ywz() const {
	return Vector3f(y, w, z);
}

Vector3f Vector4f::zxy()  const {
	return Vector3f(z, x, y);
}
Vector3f Vector4f::zyx() const {
	return Vector3f(z, y, x);
}
Vector3f Vector4f::zxw() const {
	return Vector3f(z, x, w);
}
Vector3f Vector4f::zwx() const {
	return Vector3f(z, w, x);
}
Vector3f Vector4f::zyw() const {
	return Vector3f(z, y, w);
}
Vector3f Vector4f::zwy() const {
	return Vector3f(z, w, y);
}

Vector3f Vector4f::wxy() const {
	return Vector3f(w, x, y);
}
Vector3f Vector4f::wyx() const {
	return Vector3f(w, y, x);
}
Vector3f Vector4f::wxz() const {
	return Vector3f(w, x, z);
}
Vector3f Vector4f::wzx() const {
	return Vector3f(w, z, x);
}
Vector3f Vector4f::wyz()  const {
	return Vector3f(w, y, z);
}
Vector3f Vector4f::wzy() const {
	return Vector3f(w, z, y);
}

Vector4f Vector4f::xooo() const { return Vector4f(x,0.f,0.f,0.f); }
Vector4f Vector4f::oyoo() const { return Vector4f(0.f, y, 0.f, 0.f); }
Vector4f Vector4f::oozo() const { return Vector4f(0.f, 0.f, z, 0.f); }
Vector4f Vector4f::ooow() const { return Vector4f(0.f, 0.f, 0.f, w); }

Vector4f Vector4f::xyzo() const { return Vector4f(x, y, z, 0.f); }

Vector4f Vector4f::nx() const { return Vector4f(-x, y, z, w); }
Vector4f Vector4f::ny() const { return Vector4f(x, -y, z, w); }
Vector4f Vector4f::nz() const { return Vector4f(x, y, -z, w); }
Vector4f Vector4f::nw() const { return Vector4f(x, y, z, -w); }

Vector4f Vector4f::mx(float v) const { return { v, y, z, w }; }
Vector4f Vector4f::my(float v) const { return { x, v, z, w }; }
Vector4f Vector4f::mz(float v) const { return { x, y, v, w }; }
Vector4f Vector4f::mw(float v) const { return { x, y, z, v }; }


float Vector4f::r() const { return x; }
float Vector4f::g() const { return y; }
float Vector4f::b() const { return z; }
float Vector4f::a() const { return w; }


Vector4f Vector4f::r(float v) const { return mx(v); }
Vector4f Vector4f::g(float v) const { return my(v); }
Vector4f Vector4f::b(float v) const { return mz(v); }
Vector4f Vector4f::a(float v) const { return mw(v); }

Vector2f Vector4f::gr() const { return { y, x }; }
Vector2f Vector4f::gb() const { return { y, z }; }
Vector2f Vector4f::ga() const { return { y, w }; }

Vector2f Vector4f::br() const { return { z, x }; }
Vector2f Vector4f::bg() const { return { z, y }; }
Vector2f Vector4f::ba() const { return { z, w }; }

Vector2f Vector4f::ar() const { return { w, x }; }
Vector2f Vector4f::ag() const { return { y, y }; }
Vector2f Vector4f::ab() const { return { w, z }; }

Vector2f Vector4f::rg() const { return { x, y }; }
Vector2f Vector4f::rb() const { return { x, z }; }
Vector2f Vector4f::ra() const { return { x, w }; }

Vector3f Vector4f::rgb() const { return { x, y, z }; }
Vector3f Vector4f::rbg() const { return { x, z, y }; }
Vector3f Vector4f::rga() const { return { x, y, w }; }
Vector3f Vector4f::rag() const { return { x, w, y }; }
Vector3f Vector4f::rba() const { return { x, z, w }; }
Vector3f Vector4f::rab() const { return { x, w, y }; }

Vector3f Vector4f::grb() const { return { y, x, z }; }
Vector3f Vector4f::gbr() const { return { y, z, x }; }
Vector3f Vector4f::gra() const { return { y, x, w }; }
Vector3f Vector4f::gar() const { return { y, w, x }; }
Vector3f Vector4f::gba() const { return { y, z, w }; }
Vector3f Vector4f::gab() const { return { y, w, z }; }

Vector3f Vector4f::brg() const { return { z, x, y }; }
Vector3f Vector4f::bgr() const { return { z, y, x }; }
Vector3f Vector4f::bra() const { return { z, x, w }; }
Vector3f Vector4f::bar() const { return { z, w, x }; }
Vector3f Vector4f::bga() const { return { z, y, w }; }
Vector3f Vector4f::bag() const { return { z, w, y }; }

Vector3f Vector4f::arg() const { return { w, x, y }; }
Vector3f Vector4f::agr() const { return { w, y, x }; }
Vector3f Vector4f::arb() const { return { w, x, z }; }
Vector3f Vector4f::abr() const { return { w, z, x }; }
Vector3f Vector4f::agb() const { return { w, y, z }; }
Vector3f Vector4f::abg() const { return { w, z, y }; }

Vector4f vec4x(float f) { return { f, 0.f, 0.f, 0.f }; }
Vector4f vec4y(float f) { return { 0.f, f, 0.f, 0.f }; }
Vector4f vec4z(float f) { return { 0.f, 0.f, f, 0.f }; }
Vector4f vec4w(float f) { return { 0.f, 0.f, 0.f, f }; }

Vector4f vec4xy(float x, float y) { return { x, y, 0.f, 0.f }; } Vector4f vec4xy(vec2 xy) { return { xy, 0.f, 0.f }; }
Vector4f vec4xz(float x, float z) { return { x, 0.f, z, 0.f }; } Vector4f vec4xz(vec2 xz) { return { xz.x, 0.f, xz.y, 0.f }; }
Vector4f vec4xw(float x, float w) { return { x, 0.f, 0.f, w }; } Vector4f vec4xw(vec2 xw) { return { xw.x, 0.f, 0.f, xw.y }; }

Vector4f vec4yz(float y, float z) { return { 0.f, y, z, 0.f }; } Vector4f vec4yz(vec2 yz) { return { 0.f, yz, 0.f }; }
Vector4f vec4yw(float y, float w) { return { 0.f, y, 0.f, w }; } Vector4f vec4yw(vec2 yw) { return { 0.f, yw.x, 0.f, yw.y }; }

Vector4f vec4zw(float z, float w) { return { 0.f, 0.f, z, w }; } Vector4f vec4zw(vec2 zw) { return { 0.f, 0.f, zw }; }

Vector4f vec4xyz(float x, float y, float z) { return { x, y, z, 0.f }; } Vector4f vec4xyz(vec3 xyz) { return { xyz, 0.f }; }
Vector4f vec4xyw(float x, float y, float w) { return { x, y, 0.f, w }; } Vector4f vec4xyw(vec3 xyw) { return { xyw.xy(), 0.f, xyw.z }; }
Vector4f vec4xzw(float x, float z, float w) { return { x, 0.f, z, w }; } Vector4f vec4xzw(vec3 xzw) { return { xzw.x, 0.f, xzw.yz() }; }

Vector4f vec4yzw(float y, float z, float w) { return { 0.f, y, z, w }; } Vector4f vec4yzw(vec3 yzw) { return { 0.f, yzw }; }

Vector4f vec4xfff(float x, float f) { return { x, f, f, f }; } Vector4f vec4xfff(vec2 xf) { return { xf.x, vec3(xf.y) }; }
Vector4f vec4fyff(float y, float f) { return { f, y, f, f }; }  Vector4f vec4fyff(vec2 yf) { return { yf.y, yf.x, vec2(yf.y) }; }
Vector4f vec4ffzf(float z, float f) { return { f, f, z, f }; } Vector4f vec4ffzf(vec2 zf) { return { vec2(zf.y), zf.x, zf.y }; }
Vector4f vec4fffw(float w, float f) { return { f, f, f, w }; } Vector4f vec4fffw(vec2 wf) { return { vec3(wf.y), wf.x }; }

namespace vColor4f {
	Vector4f white(1.0f);
	Vector4f black(0.0f,0.0f,0.0f,1.0f);
	Vector4f red(1.0f, 0.0f, 0.0f, 1.0f);
	Vector4f blue(0.0f, 0.0f, 1.0f, 1.0f);
	Vector4f green(0.0f, 1.0f, 0.0f, 1.0f);
	Vector4f cyan(0.0f, 1.0f, 1.0f, 1.0f);
	Vector4f orange(1.0f, 0.64453125f, 0.0f, 1.0f);
	Vector4f purple(0.5f, 0.0f, 0.5f, 1.0f);
	Vector4f magenta(1.0f, 0.0f, 1.0f, 1.0f);
	Vector4f gray(0.5f, 0.5f, 0.5f, 1.0f);

	Vector4f rand() { return { util::frand(),util::frand(),util::frand(), util::frand() }; }
}

vec4 vec4rand() { return { util::frand(), util::frand(), util::frand(), util::frand() }; }
