#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vector2f.h"
#include "Vector3f.h"

#include "Utils2D/AxisAlignedBoundingBox2D.h"

class Matrix3f{
public:
	class mRow {
		friend class Matrix3f;
		Matrix3f& parent;
		int row;

		mRow(Matrix3f& parent_, int row_) : parent(parent_), row(row_) {}

	public:
		float& operator[](int col) { return parent.m[row][col]; }
	};

	mRow operator[] (int row) {
		return mRow(*this, row);
	}

	float m[3][3];

	Matrix3f();
	Matrix3f(float m[3][3]);
	Matrix3f(float f);
	Matrix3f(vec3 v0, vec3 v1, vec3 v2);

	Matrix3f operator+ (const Matrix3f& m) const;
	Matrix3f operator- (const Matrix3f& m) const;
	Matrix3f operator* (const Matrix3f& m) const;

	Vector3f operator* (const Vector3f& m) const;

	Vector3f operator* (const Vector2f& m) const;

	AxisAlignedBoundingBox2D operator* (const AxisAlignedBoundingBox2D& bb) const;

	Matrix3f operator*= (const Matrix3f& m);

	Matrix3f transposed() const;
	Matrix3f inverted() const;

	//extracts the translation component of this matrix as a vec2
	vec2 translationv() const;
	//extracts the scaling component of this matrix as a vec2
	vec2 scalingv() const;
	
	//extracts the translation component of this matrix as a pure translation matrix
	Matrix3f translation() const;
	//extracts the translation component of this matrix as a pure rotation matrix
	Matrix3f rotation() const;
	//extracts the translation component of this matrix as a pure scaling matrix
	Matrix3f scaling() const;

	//Matrix3f translate(vec2 v);
	//Matrix3f rotate(float angle);
	//Matrix3f scale(vec2 v);

	std::string toString() const;
};
std::ostream& operator<<(std::ostream& os, const Matrix3f& mat);
std::wostream& operator<<(std::wostream& os, const Matrix3f& mat);

typedef Matrix3f mat3;

namespace pmat3 {
	mat3 translate(vec2 v);
	mat3 rotate(float angle);
	mat3 scale(vec2 v);

	//https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati
	//https://github.com/g-truc/glm/blob/master/glm/gtx/matrix_decompose.inl
	//these decompositions work only for translation, rotation and scale matrices, they do not take into account shear and perspective

	vec2 getTranslationv(mat3 const& m);
	vec2 getScalev(mat3 const& m);

	mat3 getTranslation(mat3 const& m);
	mat3 getRotation(mat3 const& m);
	mat3 getScale(mat3 const& m);

	mat3 lookAt(vec2 eye, vec2 target);

	mat3 ortho(float left, float right, float bottom, float top);
	mat3 orthoAspectX(vec2 size);
	mat3 orthoAspectY(vec2 size);
	mat3 orthoMaxAspect(vec2 size);
	mat3 orthoMinAspect(vec2 size);

	mat3 fromNdc(aabb2 const& bb);
	mat3 toNdc(aabb2 const& bb);

	//creates a normalized-device-coordinate space from a bounding box
	mat3 getNdc(AxisAlignedBoundingBox2D const& bb);
}
