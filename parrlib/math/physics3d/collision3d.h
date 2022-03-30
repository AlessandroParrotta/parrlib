#pragma once

#include "../Vector2f.h"

#include "../Matrix3f.h"

namespace prb {
	class Collider3D;

	class Collision3D {
	public:
		Vector3f axis;
		float overlap;

		Collision3D(Vector3f const& axis, float const& overlap);
		~Collision3D();

		struct solvedcol { vec3 correction1, correction2; };
		virtual solvedcol solve();
	};

	typedef Collision3D coll3;
}
