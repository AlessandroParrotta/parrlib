#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>
#endif

#include <math.h>
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

#include <locale>
#include <codecvt>
#include <string>
#include <ShellScalingApi.h>

#include <fstream>

#include "vector3f.h"
#include "time.h"
#include "../constants.h"

namespace prb {
	class Vector2f;
	class Vector3f;
	class Vector4f;

	class Matrix2f;
	class Matrix3f;
	class Matrix4f;

	namespace stringutils {
		std::string tostr(std::wstring const& str);
	}
	namespace stru = stringutils;

	namespace otherutil {

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

		//float frand();

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

		float fmin(float f, float m);
		float fmax(float f, float m);
		float clamp(float f, float min, float max);

		float dmin(float d, float m);
		float dmax(float d, float m);
		float clamp(double d, double min, double max);

		inline int pmod(const int& i, const int& n) {
			return (i % n + n) % n;
		}

		int writeToBMP(const std::string& fileName, const std::vector<std::vector<Vector3f>>& pixels);

		template<typename T>
		void concatR(std::vector<T>& a, std::vector<T> const& b) {
			a.insert(a.end(), b.begin(), b.end());
		}

		template<typename T>
		std::vector<T> concat(std::vector<T> const& a, std::vector<T> const& b) {
			std::vector<T> c(a); c.reserve(a.size() + b.size()); c.insert(c.end(), b.begin(), b.end()); return c;
		}

		std::wstring toWString(std::string str);



		int wrap(int it, int size);
		int gowrap(int it, int inc, int size);
		int incwrap(int it, int size);
		int decwrap(int it, int size);

		struct wrapIterator {
			int startPos = 0;
			int size = 0;
			int i = 0;

			wrapIterator() {}
			template<class T> wrapIterator(std::vector<T> const& vec) { this->size = vec.size(); this->startPos = wrap(startPos, size); i = wrap(startPos, size); }
			wrapIterator(int startPos, int size) { this->size = size; this->startPos = wrap(startPos, size); i = wrap(startPos, size); }
			wrapIterator(int startPos, int i, int size) { this->size = size; this->startPos = wrap(startPos, size); this->i = wrap(i, size); }
			
			int delta(int dt)	const { return wrap(i + dt, size); }
			int next()			const { return delta(1); }
			int prev()			const { return delta(-1); }

			void inc() { i = next(); }
			void dec() { i = prev(); }
			void move(int dt) { i = delta(dt); }
			void set(int i) { this->i = wrap(i, size); }
			int cur() const { return i; }

			wrapIterator& operator++() { inc();		return *this; }
			wrapIterator operator++(int) { inc();		return wrapIterator(startPos, i, size); }
			wrapIterator& operator--() { dec();		return *this; }
			wrapIterator operator--(int) { dec();		return wrapIterator(startPos, i, size); }
			wrapIterator& operator+=(const int& i) { move(i);		return *this; }
			wrapIterator& operator-=(const int& i) { move(-i);		return *this; }
			wrapIterator operator+(const int& i) const { return wrapIterator(startPos, this->i + i, size); }
			wrapIterator operator-(const int& i) const { return wrapIterator(startPos, this->i - i, size); }

			wrapIterator& operator=(const int& i) { *this = wrapIterator(i, i, size); return *this; }
			template<class T> wrapIterator& operator=(const std::vector<T>& vec) { *this = wrapIterator(0, 0, vec.size()); return *this; }

			int operator[](const int& i) const { return delta(i); }

			bool operator<(const int& i) const { return this->i < i; }
			bool operator>(const int& i) const { return this->i > i; }
			bool operator<=(const int& i) const { return this->i <= i; }
			bool operator>=(const int& i) const { return this->i >= i; }
			bool operator==(const int& i) const { return this->i == i; }
			bool operator!=(const int& i) const { return this->i != i; }

			bool operator<(const wrapIterator& i) const { return this->i < i.i; }
			bool operator>(const wrapIterator& i) const { return this->i > i.i; }
			bool operator<=(const wrapIterator& i) const { return this->i <= i.i; }
			bool operator>=(const wrapIterator& i) const { return this->i >= i.i; }
			bool operator==(const wrapIterator& i) const { return this->i == i.i; }
			bool operator!=(const wrapIterator& i) const { return this->i != i.i; }

			operator int() const { return i; }
		};
		std::ostream& operator<<(std::ostream& ot, wrapIterator const& i);
		std::wostream& operator<<(std::wostream& ot, wrapIterator const& i);
		typedef wrapIterator wrapi;

		void sleepUS(int milliseconds);
		void sleepPreciseUS(double microseconds);

		void sleepMS(int milliseconds);
		void sleepPreciseMS(double milliseconds);

		template<typename T, typename B> constexpr B smallestCheck(T at, B ab, T bt, B bb, T ct, B cb) { return at < bt ? ct < at ? cb : ab : ct < bt ? cb : bb; }
		template<typename T> constexpr T smallestCheck(T a, T b, T c) { return smallestCheck<T, T>(a, a, b, b, c, c); }

		struct path {
			std::vector<std::wstring> els;
			bool isDirectory = false;

			bool isSlash(wchar_t c) const { return c == '\\' || c == '/'; }

			path(std::vector<std::wstring> els) { this->els = els; isDirectory = get(-1).find(L".") == std::wstring::npos; }
			path(std::wstring inPath) {
				if (inPath.length() <= 0) return;
				if (inPath.length() >= 2 && inPath[0] == L'\"' && inPath[inPath.length() - 1] == L'\"') inPath = inPath.substr(1, inPath.length() - 2);

				int i = isSlash(inPath[0]) ? 1 : 0;

				while (i < inPath.length()) {
					std::wstring el = L"";
					while (i < inPath.length() && !isSlash(inPath[i])) {
						el += inPath[i];
						i++;
					}
					if (el.length() > 0) els.push_back(el);

					i++;
				}
				isDirectory = get(-1).find(L".") == std::wstring::npos;

				// for(int i=0; i<els.size(); i++) prw("el[",i,"]: ", els[i], "\n");
				// prw("is directory: ", isDirectory, "\n");
			}
			path(const wchar_t* str) : path((std::wstring)str) {}

			std::wstring get(int i) const { if (i < 0) i = els.size() + i; return els[i]; }
			path subStart(std::wstring name) const {
				std::vector<std::wstring> subpath;

				int istart = 0;
				for (int i = 0; i < els.size(); i++) if (els[i].compare(name) == 0) istart = i;
				for (int i = istart; i < els.size(); i++) subpath.push_back(els[i]);

				return path(subpath);
			}
			path subEnd(std::wstring name) const {
				std::vector<std::wstring> subpath;

				for (int i = 0; i < els.size(); i++) { subpath.push_back(els[i]); if (els[i].compare(name) == 0) break; }

				return path(subpath);
			}
			path sub(int start, int end) const {
				if (start < 0) start = els.size() + start;
				if (end < 0) end = els.size() + end;
				if (start > end) { int t = start; start = end; end = t; }
				end = end < els.size() ? end : els.size();

				std::vector<std::wstring> subpath;

				for (int i = start; i < end; i++) { subpath.push_back(els[i]); }

				return subpath;
			}

			int find(int el, std::wstring const& wstr) const { return get(el).find(wstr); }
			int find(std::wstring const& wstr) const {
				int found = -1;
				for (int i = 0; i < els.size() && found == -1; i++) if (els[i].find(wstr) != std::wstring::npos) found = i;
				return found;
			}

			std::wstring operator[] (int i) const { return get(i); }

			path operator() (int start, int end) const {
				return sub(start, end);
			}

			path operator[] (std::wstring const& start) const {
				if (start.length() > 0) {
					wchar_t sym = start[0];
					if (sym == '<') return subEnd(start.substr(2, start.length() - 2));
					else if (sym == '>') return subStart(start.substr(2, start.length() - 2));
					else return subStart(start);
				}
				else return subStart(start);
			}

			std::wstring toStringw() const {
				std::wstring str = L"";
				for (int i = 0; i < els.size(); i++) str += els[i] + L"\\";
				if (!isDirectory) str.erase(str.end() - 1);
				return str;
			}
			std::string toString() const { return stru::tostr(toStringw()); }

			operator std::wstring() const { return toStringw(); }
			operator std::string() const { return toString(); }

			std::wstring filename() const { return get(-1); }

			path parent() { return (*this)(0, -1); }

			void iterateFiles(std::function<int(path const&)> const& func) const {
				if (!isDirectory) return;
				for (auto const& dir_entry : std::filesystem::directory_iterator(toStringw())) {
					int res = func(path(dir_entry.path().c_str()));
					if (res != 0) break;
				}

				// iterateFolder(toStringw(), func); 
			}

			void iterateLines(std::function<int(std::wstring const&)> const& func) {
				if (isDirectory) return;

				std::wifstream f(toStringw().c_str());
				if (!f) { f.close(); return; }

				std::wstring line;
				while (f.good() && !f.eof()) {
					std::getline(f, line);
					int res = func(line);
					if (res != 0) break;
				}

				f.close();
			}

		};
		inline std::ostream& operator<<(std::ostream& os, const path& p) { os << p.toString(); return os; }
		inline std::wostream& operator<<(std::wostream& os, const path& p) { os << p.toStringw(); return os; }

		inline void iterateFolder(path const& path, std::function<int(prb::otherutil::path const&)> const& func) {
			path.iterateFiles(func);
		}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		HWND findMainWindow(unsigned long process_id);
		HWND findHiddenWindow(unsigned long process_id);
		HWND findWindowByClassName(std::wstring _class, std::wstring name);

		struct AsyncProgram {
			HANDLE hProcess;
			HANDLE hThread;
			DWORD dwProcessID;
			DWORD dwThreadID;
		};
		AsyncProgram openProgramAsync(std::wstring exe, std::wstring arg);
		void closeProgramAsync(AsyncProgram prog);
		int openProgram(std::wstring exe, std::wstring arg);
		int openCMD(std::wstring arg);
		std::string openProgramRedirected(std::wstring exe, std::wstring args);

		std::wstring ExePath();
		bool isInvokedByCMD();
		std::string getExeLocation();
		std::string getExeFolder();

		float monitorScalingToPerc(DEVICE_SCALE_FACTOR dsf);

		template<typename T> inline std::vector<T> subV(std::vector<T> const& vec, int start, int end) {
			if (start < 0) start = vec.size() + start;
			if (end < 0) end = vec.size() + end;
			return std::vector<T>(vec.begin() + start, vec.begin() + end + 1);
		}
#endif
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace outl = otherutil;
#endif
}
