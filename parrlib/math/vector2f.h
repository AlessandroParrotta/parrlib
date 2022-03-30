#pragma once
#include <string>

namespace prb {
	class Vector3f;

	class Vector2f
	{
	public:
		float x = 0.0f, y = 0.0f;

		Vector2f operator+ (const Vector2f& other) const;
		Vector2f operator- (const Vector2f& other) const;
		Vector2f operator* (const Vector2f& other) const;
		Vector2f operator/ (const Vector2f& other) const;

		Vector2f operator+= (const Vector2f& other);
		Vector2f operator-= (const Vector2f& other);
		Vector2f operator*= (const Vector2f& other);
		Vector2f operator/= (const Vector2f& other);

		Vector2f operator+ (float other) const;
		Vector2f operator- (float other) const;
		Vector2f operator* (float other) const;
		Vector2f operator/ (float other) const;

		Vector2f operator+= (float other);
		Vector2f operator-= (float other);
		Vector2f operator*= (float other);
		Vector2f operator/= (float other);

		bool operator> (const Vector2f& other) const;
		bool operator< (const Vector2f& other) const;
		bool operator>= (const Vector2f& other) const;
		bool operator<= (const Vector2f& other) const;
		bool operator== (const Vector2f& other) const;
		bool operator!= (const Vector2f& other) const;

		float& get(int i);
		float get(int i) const;

		float& operator[](int i);
		float operator[](int i) const;

		constexpr unsigned int length() const { return 2; }

		bool equal(Vector2f const& v) const;				//equivalent to doing vec2 == v
		bool equal(Vector2f const& v, float bias) const;	//returns true if this vec is equal to 'v' within an interval of 'bias'

		float magnitude() const;
		float dot(Vector2f const& other) const;
		float det(Vector2f const& v) const;
		Vector2f normalized() const;
		float cross(Vector2f const& v) const;
		Vector2f perp() const;
		Vector2f abs() const;
		Vector2f lerp(Vector2f const& v, float t) const;

		Vector2f minned(Vector2f val) const;	//returns { min(x, val.x), min(y, val.y) }
		Vector2f minned(float val) const;		//returns { min(x, val), min(y, val) }

		Vector2f maxed(Vector2f val) const;		//returns { max(x, val.x), max(y, val.y) }
		Vector2f maxed(float val) const;		//returns { max(x, val), max(y, val) }

		Vector2f floored() const;
		Vector2f ceiled() const;

		Vector2f clamped(float min, float max) const;
		Vector2f clamped(Vector2f range) const;
		Vector2f clamped(Vector2f rangex, Vector2f rangey) const;

		Vector2f modded(Vector2f v) const;

		float aspectx() const;
		float aspecty() const;
		float aspectmax() const;
		float aspectmin() const;
		Vector2f aspectmaxv() const;
		Vector2f aspectminv() const;
		Vector2f aspectxy() const;

		//return the maximum value between x and y
		float maxv() const;
		//return the minimum value between x and y
		float minv() const;

		Vector2f invy(float ref) const;
		Vector2f invx(float ref) const;
		Vector2f inv(Vector2f ref) const;

		Vector2f rotate(Vector2f pivot, float angle) const;
		Vector2f rotate(Vector2f pivot, float angle, float ortho) const;
		Vector2f rotate(float angle) const;

		bool equalsRange(Vector2f v, Vector2f range) const;
		bool equalsRange(Vector2f v, float range) const;

		bool inRange(Vector2f min, Vector2f max) const;

		Vector2f getClone() const;

		std::string toString() const;
		std::wstring toWString() const;

		Vector2f xf(float f) const;
		Vector2f fy(float f) const;

		Vector2f mx(float f) const; //offset from x
		Vector2f my(float f) const; //offset from y

		Vector2f yx() const;

		Vector2f xx() const;
		Vector2f yy() const;

		Vector2f nx() const;
		Vector2f ny() const;

		Vector2f xo() const;
		Vector2f oy() const;
		Vector2f yo() const;
		Vector2f ox() const;

		Vector2f();
		Vector2f(float xy);

		Vector2f(float x, float y);
		//Vector2f(int x, int y);
		//Vector2f(unsigned int x, unsigned int y);
		//Vector2f(double x, double y);
		//Vector2f(char x, char y);

		Vector2f(Vector3f v);

		~Vector2f();
	};

	Vector2f operator+ (float f, Vector2f v);
	Vector2f operator- (float f, Vector2f v);
	Vector2f operator* (float f, Vector2f v);
	Vector2f operator/ (float f, Vector2f v);

	Vector2f operator- (Vector2f v);

	std::ostream& operator<<(std::ostream& os, Vector2f const& v);
	std::wostream& operator<<(std::wostream& os, Vector2f const& v);

	typedef Vector2f vec2;

	vec2 vec2x(float x);
	vec2 vec2y(float y);

	vec2 vec2rand();

	namespace v2ax {
		const vec2 fwv2 = { 1.f, 0.f };
		const vec2 upv2 = { 0.f, 1.f };
	}

	//vec2 util
	namespace v2u {
		vec2 x(float x);
		vec2 y(float y);

		vec2 get(std::stringstream& ss);
		vec2 get(std::ifstream& f);
	}
}
