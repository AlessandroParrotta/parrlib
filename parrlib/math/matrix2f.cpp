#include "Matrix2f.h"

#include "OtherUtil.h"

#define PI 3.141592

namespace prb {
	Matrix2f::Matrix2f() {

	}

	Matrix2f::Matrix2f(float m[2][2]) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->m[i][j] = m[i][j];
			}
		}
	}

	Matrix2f::Matrix2f(float f) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->m[i][j] = 0.f;
			}
		}

		m[0][0] = f;
		m[1][1] = f;
	}

	Matrix2f::~Matrix2f() {

	}

	Matrix2f Matrix2f::operator+ (const Matrix2f& other) const {
		Matrix2f result = Matrix2f();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = this->m[i][j] + other.m[i][j];
			}
		}

		return result;
	}

	Matrix2f Matrix2f::operator- (const Matrix2f& other) const {
		Matrix2f result = Matrix2f();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = this->m[i][j] - other.m[i][j];
			}
		}

		return result;
	}

	Matrix2f Matrix2f::operator* (const Matrix2f& other) const {
		Matrix2f result = Matrix2f();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = this->m[i][0] * other.m[0][j] +
					this->m[i][1] * other.m[1][j] +
					this->m[i][2] * other.m[2][j] +
					this->m[i][3] * other.m[3][j];
			}
		}

		return result;
	}

	void Matrix2f::setupRotation(const float& angle) {
		float theta = outl::toRadians(angle);
		m[0][0] = cos(theta);
		m[0][1] = -sin(theta);
		m[1][0] = sin(theta);
		m[1][1] = cos(theta);
	}

	namespace pmat2 {
		mat2 rotate(const float& angle) {
			mat2 m = 1.f;
			m.setupRotation(angle);
			return m;
		}

		mat2 translate(const vec2& v) {
			mat2 m = 1.f;
			return m;
		}
	}
}
