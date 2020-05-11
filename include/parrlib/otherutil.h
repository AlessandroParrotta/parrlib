#pragma once

#include <math.h>
#include <vector>
#include <string>

#include <locale>
#include <codecvt>
#include <string>

#include <fstream>

#include "vector3f.h"
#include "time.h"
#include <parrlib/constants.h>

class Vector2f;
class Vector3f;
class Vector4f;

class Matrix2f;
class Matrix3f;
class Matrix4f;

namespace otherutil{
	//stringstream compatible with string and wstring
	class uniss : public std::wstringstream {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	public:
		uniss& operator<< (const char* str) {
			*this << converter.from_bytes(str);
			return *this;
		}

		uniss& operator<< (std::string const& str) {
			*this << converter.from_bytes(str);
			return *this;
		}

		uniss& operator<< (std::wstring const& v) { *((std::wstringstream*)(this)) << v; return *this; }

		uniss& operator<< (int const& v) { *((std::wstringstream*)(this)) << v; return *this; }
		uniss& operator<< (unsigned int const& v) { *((std::wstringstream*)(this)) << v; return *this; }
		uniss& operator<< (float const& v) { *((std::wstringstream*)(this)) << v; return *this; }
		uniss& operator<< (double const& v) { *((std::wstringstream*)(this)) << v; return *this; }
	};

	int loadInt(std::ifstream& f);
	float loadFloat(std::ifstream& f);
	double loadDouble(std::ifstream& f);
	char loadChar(std::ifstream& f);
	std::string loadString(std::ifstream& f);

	Vector2f loadVec2(std::ifstream& f);
	Vector3f loadVec3(std::ifstream& f);
	Vector4f loadVec4(std::ifstream& f);

	Matrix2f loadMat2(std::ifstream& f);
	Matrix3f loadMat3(std::ifstream& f);
	Matrix4f loadMat4(std::ifstream& f);

	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	int sign(int const& v);
	float sign(float const& v);
	double sign(double const& v);

	float lerp(float v, float target, float speed);
	double lerp(double v, double target, double speed);
	
	void lerpr(float& v, float target, float speed);
	void lerpr(double& v, double target, double speed);
	
	float lerprr(float& v, float target, float speed);
	double lerprr(double& v, double target, double speed);

	vec2 lerp(vec2 const& v, vec2 const& target, float speed);

	float minfabs(float const& f1, float const& f2);
	float maxfabs(float const& f1, float const& f2);

	//Normalizes any number to an arbitrary range, [-180,180], [0,180] or [0,360]
	//by assuming the range wraps around when going below min or above max 
	//https://stackoverflow.com/questions/1628386/normalise-orientation-between-0-and-360
	double normalizeDegAnyVal(const double value, const double start, const double end);
	float normalizeDeg(float deg);

	std::vector<vec2> getPointsBetweenDegrees(vec2 pos, float degs, float dege, float radius, int iters);

	float getDegDif(vec2 v0, vec2 v1);
	float getDeg(vec2 v0);
	std::vector<vec2> getSemiCircle(vec2 pos, vec2 normal, bool side, float radius, int iters);

	inline vec2 getEdge(const vec2& v0, const vec2& v1) { return v1 - v0; }
	inline vec2 getEdgen(const vec2& v0, const vec2& v1) { return (v1 - v0).normalized(); }

	vec2 getPointLinesMeet(vec2 l1s, vec2 l1e, vec2 l2s, vec2 l2e);

	//inline funcs must always be in the header file
	inline float toRadians(const float& degrees) { return (degrees * cst::PI) / 180.0f; }
	inline float toDegrees(const float& radians) { return (radians * 180.0f) / cst::PI; }

	inline float randomInterval(const float& low, const float& high) {
		return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
	}

	inline float randomInterval(const float& range) {
		return randomInterval(-range, range);
	}


	inline float frand() { return (float)rand() / (float)RAND_MAX; }


	inline int approxEquals(const float& f, const float& val, const float& low, const float& high) {
		return f < val - low ? -1 : f > val + high ? 1 : 0;
	}

	inline int approxEquals(const float& f, const float& val, const float& range) {
		return approxEquals(f, val, range, range);
	}

	inline int imin(const int& i, const int& m) { return i < m ? i : m; }
	inline int imax(const int& i, const int& m) { return i > m ? i : m; }
	inline int clamp(const int& i, const int& iimin, const int& iimax) { return imin(imax(i, iimin), iimax); }

	float clamp(float f, float min, float max);
	float clamp(double d, double min, double max);

	inline int pmod(const int& i, const int& n) {
		return (i % n + n) % n;
	}

	int writeToBMP(const std::string& fileName, const std::vector<std::vector<Vector3f>>& pixels);

	std::wstring toWString(std::string str);
}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace outl = otherutil;
#endif
