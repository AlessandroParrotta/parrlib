#pragma once

#include <iostream>
#include <cstdarg>

template<typename T, size_t L>
class VectorT {
public:
	std::array<T, L> v;

	T& x = v[0]; T& r = v[0];
	T& y = v[1]; T& g = v[1];
	T& z = v[2]; T& b = v[2];
	T& w = v[3]; T& a = v[3];

	constexpr T& operator[](size_t i) { return v[i]; }
	constexpr T operator[](size_t i) const { return v[i]; }

	constexpr VectorT() {}

	template<typename... Args>
	constexpr VectorT(Args... vals) {
		const int size = sizeof...(vals);
		static_assert(size >= L, "number of parameters must be greater or equal to length of vector");
		std::array<T, size> tarr = { vals... };
		for (int i = 0; i < L; i++) v[i] = tarr[i];
		//this solution might be unoptimized, i must find a way to expand only part of the pack 
		//directly without going through another array
	}

	template<typename TT>
	constexpr TT magnitude() const { 
		TT res = static_cast<TT>(0);
		for (int i = 0; i < L; i++) res += static_cast<TT>(v[i]) * static_cast<TT>(v[i]);
		return res;
	}
	constexpr T magnitude() const { return magnitude<T>(); }

	constexpr T dot(VectorT<T, L> const& other) const {
		T res = static_cast<T>(0);
		for (int i = 0; i < L; i++) res += v[i] * other[i];
		return res;
	}

	template<typename TT>
	constexpr VectorT<TT, L> normalized() const {
		TT magn = magnitude<TT>();
		VectorT<TT, L> res;
		for (int i = 0; i < L; i++) res[i] = static_cast<TT>(v[i]) / magn;
		return res;
	}
	constexpr VectorT<T, L> normalized() const { return normalized<T>(); }

	constexpr VectorT<T, L> abs() const { VectorT<T, L> res;  for (int i = 0; i < L; i++) res[i] = (v[i] < static_cast<T>(0) ? -v[i] : v[i]); return res; }
	constexpr size_t length() const { return L; }

	template<unsigned int LL = L>
	typename std::enable_if<LL == 2 && L == 2, T>::type
	cross(VectorT<T, 2> const& ov) {
		return v[0] * ov[1] - v[1] * ov[0];
	}

	template<unsigned int LL = L>
	VectorT<typename std::enable_if<LL == 3 && L == 3, T>::type, LL>
	cross(VectorT<T, 3> const& ov) {
		return { v[1] * ov[2] - v[2] - ov[1], v[2] * ov[0] - v[0] - ov[2], v[0] * ov[1] - v[1] * ov[0] };
	}

	std::string toString() const { std::stringstream ss ;ss << *this; return ss.str(); }
	std::wstring toWString() const { std::wstringstream ss; ss << *this; return ss.str(); }
};

template<typename T, size_t L>
constexpr std::ostream& operator<<(std::ostream& os, VectorT<T, L> v) {
	os << "(";
	for (int i = 0; i < L; i++) {
		os << v[i] << (i == L-1 ? "" : " ");
	}
	os << ")";
	return os;
}

template<typename T, size_t L>
constexpr std::wostream& operator<<(std::wostream& os, VectorT<T, L> v) {
	os << L"(";
	for (int i = 0; i < L; i++) {
		os << v[i] << (i == L-1 ? L"" : L" ");
	}
	os << L")";
	return os;
}

typedef VectorT<int, 2> Vector2i;
typedef Vector2i vec2i;

typedef VectorT<int, 3> Vector3i;
typedef Vector3i vec3i;

typedef VectorT<int, 4> Vector4i;
typedef Vector4i vec4i;


typedef VectorT<double, 2> Vector2d;
typedef Vector2d vec2d;

typedef VectorT<double, 3> Vector3d;
typedef Vector3d vec3d;

typedef VectorT<double, 4> Vector4d;
typedef Vector4d vec4d;
