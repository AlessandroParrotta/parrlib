#pragma once

#include <math.h>

#include "Vector2f.h"

namespace prb {
	class Matrix2f
	{
	public:
		float m[2][2];

		Matrix2f();
		Matrix2f(float m[2][2]);
		Matrix2f(float f);

		~Matrix2f();

		Matrix2f operator+ (const Matrix2f& other) const;
		Matrix2f operator- (const Matrix2f& other) const;
		Matrix2f operator* (const Matrix2f& other) const;

		void setupRotation(const float& angle);
	};

	typedef Matrix2f mat2;

	namespace pmat2 {
		mat2 translate(vec2 v);
		mat2 rotate(const float& angle);
		mat2 scale(vec2 v);
	}
}
