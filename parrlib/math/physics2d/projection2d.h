#pragma once

#include <vector>

#include "../Vector2f.h"
#include "../Utils2D/AxisAlignedBoundingBox2D.h"
#include "../Matrix3f.h"

namespace prb {
	class Collider2D;

	class Projection2D {
	public:
		float min, max;

		Projection2D(float min, float max);
		Projection2D(std::vector<vec2> const& vs, mat3 const& tr, Vector2f const& axis);
		Projection2D(vec2 const& v1, vec2 const& v2, Vector2f const& axis);
		Projection2D(aabb2 const& bb, vec2 const& axis);
		Projection2D(Collider2D const& col, mat3 const& tr, Vector2f const& axis);

		~Projection2D();

		float getOverlap(Projection2D p);
		bool overlaps(Projection2D p);
		int overlapsI(Projection2D p);
	};

	typedef Projection2D proj2;
}
