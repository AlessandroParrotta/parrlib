#pragma once

#include <vector>

#include "../Vector2f.h"

#include "../Utils2D/Utils2D.h"

#include "Collision2d.h"
#include "Projection2d.h"

namespace prb {
	class Collider2D {
	public:
		float mass = 1.0f;

		std::vector<Vector2f> verts;
		std::vector<vec2> axes;

		Collider2D(std::vector<Vector2f> const& verts);
		Collider2D(std::vector<Vector2f> const& verts, std::vector<vec2> const& axes);

		~Collider2D();

		virtual std::shared_ptr<Collision2D> checkCollision(mat3 const& tr, Collider2D* other, mat3 const& otr);
	};

	typedef Collider2D col2;
}
