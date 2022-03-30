#pragma once

#include "../Vector2f.h"

#include "../Utils2D/Consts.h"
#include "../Matrix3f.h"

namespace prb {
	class Collider2D;

	class Collision2D {
	public:
		Vector2f axis;
		float overlap;

		Collision2D(Vector2f const& axis, float const& overlap);
		~Collision2D();

		struct solvedcol { vec2 correction1, correction2; };
		virtual solvedcol solve();
	};

	typedef Collision2D coll2;
}
