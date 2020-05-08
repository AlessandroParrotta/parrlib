#include "Matrix3f.h"
#include <iostream>

#include "stringutils.h"

Matrix3f::Matrix3f() {}

Matrix3f::Matrix3f(float m[3][3]) {

}

Matrix3f::Matrix3f(float f) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->m[i][j] = 0.f;
		}
	}

	m[0][0] = f;
	m[1][1] = f;
	m[2][2] = f;
}

Matrix3f::Matrix3f(vec3 v0, vec3 v1, vec3 v2) {
	m[0][0] = v0.x; m[0][1] = v0.y; m[0][2] = v0.z;
	m[1][0] = v1.x; m[1][1] = v1.y; m[1][2] = v1.z;
	m[2][0] = v2.x; m[2][1] = v2.y; m[2][2] = v2.z;
}

Matrix3f Matrix3f::operator+ (const Matrix3f& m) const {
	mat3 rm = 0.f;
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) rm[i][j] = this->m[i][j] + m.m[i][j];
	return rm;
}

Matrix3f Matrix3f::operator- (const Matrix3f& m) const {
	mat3 rm = 0.f;
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) rm[i][j] = this->m[i][j] - m.m[i][j];
	return rm;
}

Matrix3f Matrix3f::operator* (const Matrix3f& m) const {
	mat3 result = 0.f;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = 0.f;
			for (int k = 0; k < 3; k++) {
				result.m[i][j] += m.m[i][k] * this->m[k][j];
			}
		}
	}
	return result;
}

Vector3f Matrix3f::operator* (const Vector3f& v) const {
	vec3 ris;
	ris = {
		v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
		v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
		v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2],
	};
	return ris;
}

Vector3f Matrix3f::operator* (const Vector2f& v) const {
	return *this * vec3(v, 1.f);
}

AxisAlignedBoundingBox2D Matrix3f::operator* (const AxisAlignedBoundingBox2D& bb) const {
	return AxisAlignedBoundingBox2D(*this * bb.fmin(), *this * bb.fmax());
}

Matrix3f Matrix3f::transposed() const {
	Matrix3f rm;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			rm[i][j] = m[j][i];
		}
	}
	return rm;
}

Matrix3f Matrix3f::operator*= (const Matrix3f& m) {
	*this = *this * m;
	return *this;
}

//from https://github.com/g-truc/glm/blob/master/glm/detail/func_matrix.inl
Matrix3f Matrix3f::inverted() const {
	float OneOverDeterminant = 1.f / (
		+ m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	Matrix3f Inverse;
	Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
	Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
	Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
	Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
	Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
	Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
	Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
	Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
	Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

	return Inverse;
}

vec2 Matrix3f::translationv() const { return vec2(m[0][2], m[1][2]); }
vec2 Matrix3f::scalingv() const {
	return vec2(
		vec2(m[0][0], m[1][0]).magnitude(),
		vec2(m[0][1], m[1][1]).magnitude()
	);
}

Matrix3f Matrix3f::translation() const {
	mat3 r = 1.f;
	r[0][2] = m[0][2];
	r[1][2] = m[1][2];
	return r;
}
Matrix3f Matrix3f::rotation() const {
	vec2 scale = scalingv();
	mat3 r = 1.f;
	r[0][0] = m[0][0] * scale.x;
	r[1][0] = m[1][0] * scale.x;
	r[0][1] = m[0][1] * scale.y;
	r[1][1] = m[1][1] * scale.y;
	return r;
}
Matrix3f Matrix3f::scaling() const {
	mat3 r = 1.f;
	r[0][0] = vec2(m[0][0], m[1][0]).magnitude();
	r[1][1] = vec2(m[0][1], m[1][1]).magnitude();
	return r;
}

std::string Matrix3f::toString() const {
	std::stringstream ss;
	ss << "(\n";

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			ss << m[i][j];
			ss << " ";
		}
		ss << "\n";
	}
	ss << ")";
	return ss.str();
}
std::ostream& operator<<(std::ostream& os, const Matrix3f& mat) { return os << mat.toString(); }
std::wostream& operator<<(std::wostream& os, const Matrix3f& mat) { return os << stru::towstr(mat.toString()); }

namespace pmat3 {
	mat3 translate(vec2 v) {
		mat3 m = 1.f;
		m[0][2] = v.x;
		m[1][2] = v.y;
		return m.transposed();
	}

	mat3 rotate(float angle) {
		angle = angle * M_PI / 180.f;  //convert to radians
		mat3 m = 0.f;
		m[0][0] = cos(angle); m[0][1] = -sin(angle);
		m[1][0] = sin(angle); m[1][1] = cos(angle);
		m[2][2] = 1.f;
		return m.transposed();
	}

	mat3 scale(vec2 v) {
		mat3 m = 0.f;
		m[0][0] = v.x;
		m[1][1] = v.y;
		m[2][2] = 1.f;
		return m.transposed();
	}

	vec2 getTranslationv(mat3 const& m) { return vec2(m.m[0][2], m.m[1][2]); }
	vec2 getScalev(mat3 const& m) {return m.scalingv();}

	mat3 getTranslation(mat3 const& m) {return m.translation();}
	mat3 getRotation(mat3 const& m) {return m.rotation();}
	mat3 getScale(mat3 const& m) {return m.scaling();}

	mat3 lookAt(vec2 eye, vec2 target) {
		if (eye == target) rotate(0.f);

		vec2 dir = (target - eye).normalized();
		float dot = eye.x * target.x + eye.y * target.y;
		float det = eye.x * target.y - eye.y * target.x;
		float angle = (atan2f(dir.y, dir.x) * 180.f) / M_PI; // to degrees
		return rotate(angle);
	}

	mat3 ortho(float left, float right, float bottom, float top) {
		mat3 mat = 1.f;
		mat[0][0] = 2.f / (right - left);
		mat[1][1] = 2.f / (top - bottom);
		mat[2][0] = -(right + left) / (right - left);
		mat[2][1] = -(top + bottom) / (top - bottom);
		return mat;
	}

	mat3 orthoAspectX(vec2 size) { float a = size.aspectx(); return ortho(-1.f * a, 1.f * a, -1.f, 1.f); }
	mat3 orthoAspectY(vec2 size) { float a = size.aspecty(); return ortho(-1.f, 1.f, -1.f*a, 1.f*a); }
	mat3 orthoMaxAspect(vec2 size) { 
		if (std::fabs(size.aspectx()) > std::fabs(size.aspecty()))	return orthoAspectX(size);
		else														return orthoAspectY(size);
	}
	mat3 orthoMinAspect(vec2 size) { 
		if (std::fabs(size.aspectx()) < std::fabs(size.aspecty()))	return orthoAspectX(size);
		else														return orthoAspectY(size);
	}

	mat3 fromNdc(aabb2 const& bb) { return translate(bb.center()) * scale(bb.size() / 2.f); }
	//mat3 toNdc(aabb2 const& bb) { return scale(1.f / ((bb.fmax() - bb.fmin()) / 2.f)) * translate(-(bb.fmin() + bb.fmax()) / 2.f); }
	mat3 toNdc(aabb2 const& bb) { return scale(1.f / (bb.size()/2.f)) * translate(-bb.center()); }

	mat3 getNdc(AxisAlignedBoundingBox2D const& bb) {
		return toNdc(bb);
	}
}
