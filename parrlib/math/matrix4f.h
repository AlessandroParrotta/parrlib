#pragma once

#include <math.h>
#include <iostream>
#include <string>

#include "Vector3f.h"
#include "Vector4f.h"

namespace prb {
	class Matrix3f;

	class Matrix4f {
	public:
		class mRow {
			friend class Matrix4f;
			Matrix4f* parent = nullptr;
			const Matrix4f* parentSt = nullptr;
			int row;

			mRow(Matrix4f& parent_, int row_) : parent(&parent_), row(row_) {}
			mRow(Matrix4f const& parent_, int row_) : parentSt(&parent_), row(row_) {}

		public:
			float& operator[](int col) { return parent->matrix[row][col]; }
			float operator[](int col) const { return parentSt->matrix[row][col]; }
			mRow& operator=(const float& i) { parent->matrix[(int)std::floor((float)row / 4.f)][row % 4] = i; return *this; }
		};

		mRow operator[] (int row) { return mRow(*this, row); }
		mRow operator[] (int row) const { return mRow(*this, row); }

		float matrix[4][4];

		Matrix4f operator+ (const Matrix4f& other) const;
		Matrix4f operator- (const Matrix4f& other) const;
		Matrix4f operator* (const Matrix4f& other) const;

		Vector4f operator* (const Vector4f& other) const;
		Vector4f operator* (const Vector3f& v) const;

		Matrix4f operator* (const float& f) const;

		void getCofactor(float A[4][4], float temp[4][4], int p, int q, int n);
		int determinant(float A[4][4], int n);
		void adjoint(float A[4][4], float adj[4][4]);
		//Matrix4f inverse(Matrix4f m);

		Matrix4f transposed() const;
		Matrix4f inverted() const;

		void setupIdentity();
		void setupTranslation(float x, float y, float z);
		void setupRotation(float x, float y, float z);
		void setupScale(float x, float y, float z);

		void setupTranslation(Vector3f const& trasl);
		void setupRotation(Vector3f const& rot);
		void setupRotationAxis(float angle, vec3 const& axis);
		void setupScale(Vector3f const& scale);

		void setupPerspective(float fov, float aspect, float znear, float zfar);
		void setupCamera(Vector3f const& forward, Vector3f const& up);

		vec3 toEuler() const;

		Matrix4f lerp(Matrix4f const& b, float t) const;

		vec3 translationv() const;
		vec3 scalingv() const;

		Matrix4f translation() const;
		Matrix4f rotation() const;
		Matrix4f scaling() const;

		bool isIdentity() const;
		bool isDegenerate() const;

		vec3 fw() const;
		vec3 ri() const;
		vec3 up() const;

		Matrix4f();
		Matrix4f(float matrix[4][4]);
		Matrix4f(float f);
		Matrix4f(Vector4f const& r0, Vector4f const& r1, Vector4f const& r2, Vector4f const& r3);

		~Matrix4f();

		std::string toString() const;
	};

	std::ostream& operator<<(std::ostream& os, const Matrix4f& mat);
	std::wostream& operator<<(std::wostream& os, const Matrix4f& mat);

	typedef Matrix4f mat4;

	namespace pmat4 {
		mat4 translate(vec3 const& v);
		mat4 rotate(float axis, vec3 const& v);
		mat4 rotateEuler(vec3 const& rot);		//in degrees
		mat4 scale(vec3 const& v);

		mat4 lookAt(vec3 const& eye, vec3 const& center, vec3 const& up);
		mat4 camera(vec3 const& forward, vec3 const& up);
		mat4 perspective(float fov, float aspect, float znear, float zfar);
		mat4 ortho(float left, float right, float bottom, float top, float znear, float zfar);

		mat4 inverse(mat4 const& m);

		mat4 toMat4(Matrix3f const& mat);
	}
}
