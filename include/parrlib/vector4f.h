#pragma once

#include <string>
#include <sstream>

#include "Vector2f.h"
#include "Vector3f.h"

class Vector4f{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	Vector4f right();
	Vector4f up();
	Vector4f forward();

	Vector4f operator+ (const Vector4f &other) const;
	Vector4f operator- (const Vector4f &other) const;
	Vector4f operator* (const Vector4f &other) const;
	Vector4f operator/ (const Vector4f &other) const;

	Vector4f operator+= (const Vector4f &other);
	Vector4f operator-= (const Vector4f &other);
	Vector4f operator*= (const Vector4f &other);
	Vector4f operator/= (const Vector4f &other);

	bool operator> (const Vector4f &other) const;
	bool operator< (const Vector4f &other) const;
	bool operator>= (const Vector4f &other) const;
	bool operator<= (const Vector4f &other) const;
	bool operator== (const Vector4f &other) const;
	bool operator!= (const Vector4f &other) const;

	float operator[] (int i) const;
	float& operator[] (int i);

	constexpr unsigned int length() const { return 4; }

	float magnitude() const;
	float dot(Vector4f other) const;
	Vector4f normalized() const;
	Vector4f abs() const;
	Vector4f clone() const;
	std::string toString() const;

	Vector4f colMul(float xf, float yf, float zf) const;
	Vector4f colMul(Vector3f f) const;
	Vector4f colMul(float f) const;

	Vector4f colDiv(float xf, float yf, float zf) const;
	Vector4f colDiv(Vector3f f) const;
	Vector4f colDiv(float f) const;

	Vector4f realColMul(float factor) const;
	Vector4f invCol() const;
	Vector4f negCol() const;

	//swizzle operation (see GLSL swizzle for clarification)
	//returns a new vec4 following GLSL swizzle syntax with some additions:
	//you can append 'n' to any of the operators to make them negative (e.g 'nxynzw' returns {-x,y,-z,w})
	//you can put 'o' in place of a component to zero it out (e.g 'xyoz' returns {x,y,0.f,z})
	Vector4f Vector4f::sz(std::string const& str) const;
	//Vector4f sz4(std::string const& str) const;
	//Vector3f sz3(std::string const& str) const;
	//Vector2f sz2(std::string const& str) const;

	Vector4f();
	Vector4f(float x, float y, float z, float w);

	Vector4f(Vector2f v2, float z, float w);
	Vector4f(float x, Vector2f v2, float w);
	Vector4f(float x, float y, Vector2f v2);
	Vector4f(Vector2f v1, Vector2f v2);

	Vector4f(Vector3f v3, float w);
	Vector4f(float x, Vector3f v3);

	//colors
	Vector4f(std::string hex);
	Vector4f(const char* hex);
	Vector4f(unsigned int hex);

	Vector4f(float f);
	Vector4f(float *f);

	~Vector4f();

	Vector2f yx() const;
	Vector2f yz() const;
	Vector2f yw() const;

	Vector2f zx() const;
	Vector2f zy() const;
	Vector2f zw() const;

	Vector2f wx() const;
	Vector2f wy() const;
	Vector2f wz() const;

	Vector2f xy() const;
	Vector2f xz() const;
	Vector2f xw() const;

	Vector3f xyz() const;
	Vector3f xzy() const;
	Vector3f xyw() const;
	Vector3f xwy() const;
	Vector3f xzw() const;
	Vector3f xwz() const;

	Vector3f yxz() const;
	Vector3f yzx() const;
	Vector3f yxw() const;
	Vector3f ywx() const;
	Vector3f yzw() const;
	Vector3f ywz() const;

	Vector3f zxy() const;
	Vector3f zyx() const;
	Vector3f zxw() const;
	Vector3f zwx() const;
	Vector3f zyw() const;
	Vector3f zwy() const;

	Vector3f wxy() const;
	Vector3f wyx() const;
	Vector3f wxz() const;
	Vector3f wzx() const;
	Vector3f wyz() const;
	Vector3f wzy() const;

	Vector4f xooo() const;
	Vector4f oyoo() const;
	Vector4f oozo() const;
	Vector4f ooow() const;

	Vector4f xyzo() const;

	Vector4f nx() const;
	Vector4f ny() const;
	Vector4f nz() const;
	Vector4f nw() const;

	Vector4f mx(float v) const;
	Vector4f my(float v) const;
	Vector4f mz(float v) const;
	Vector4f mw(float v) const;


	float r() const; 
	float g() const; 
	float b() const; 
	float a() const;


	Vector4f r(float v) const;
	Vector4f g(float v) const;
	Vector4f b(float v) const;
	Vector4f a(float v) const;

	Vector2f gr() const;
	Vector2f gb() const;
	Vector2f ga() const;

	Vector2f br() const;
	Vector2f bg() const;
	Vector2f ba() const;

	Vector2f ar() const;
	Vector2f ag() const;
	Vector2f ab() const;

	Vector2f rg() const;
	Vector2f rb() const;
	Vector2f ra() const;

	Vector3f rgb() const;
	Vector3f rbg() const;
	Vector3f rga() const;
	Vector3f rag() const;
	Vector3f rba() const;
	Vector3f rab() const;

	Vector3f grb() const;
	Vector3f gbr() const;
	Vector3f gra() const;
	Vector3f gar() const;
	Vector3f gba() const;
	Vector3f gab() const;

	Vector3f brg() const;
	Vector3f bgr() const;
	Vector3f bra() const;
	Vector3f bar() const;
	Vector3f bga() const;
	Vector3f bag() const;

	Vector3f arg() const;
	Vector3f agr() const;
	Vector3f arb() const;
	Vector3f abr() const;
	Vector3f agb() const;
	Vector3f abg() const;
};

Vector4f operator+ (float f, Vector4f const& v);
Vector4f operator- (float f, Vector4f const& v);
Vector4f operator* (float f, Vector4f const& v);
Vector4f operator/ (float f, Vector4f const& v);

Vector4f operator- (Vector4f const& v);

std::ostream &operator<<(std::ostream &os, Vector4f const& v);
std::wostream &operator<<(std::wostream &os, Vector4f const& v);

namespace vColor4f {
	extern Vector4f white;
	extern Vector4f black;
	extern Vector4f red;
	extern Vector4f blue;
	extern Vector4f green;
	extern Vector4f cyan;
	extern Vector4f orange;
	extern Vector4f purple;
	extern Vector4f magenta;
	extern Vector4f gray;

	Vector4f rand();
}

typedef Vector4f vec4;

vec4 vec4rand();

namespace vc4 = vColor4f;

