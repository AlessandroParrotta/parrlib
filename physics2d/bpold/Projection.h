#pragma once

#include <vector>

#include "../Vector2f.h"
#include "../Utils2D/AxisAlignedBoundingBox2D.h"

class Collider;

class Projection{
public:
	float min, max;

	Projection(float min, float max);
	Projection(std::vector<vec2> const& vs, Vector2f const& axis);
	Projection(vec2 const& v1, vec2 const& v2, Vector2f const& axis);
	Projection(aabb2 const& bb, vec2 const& axis);
	Projection(Collider const& col, Vector2f const& axis);

	~Projection();

	float Projection::getOverlap(Projection p);
	bool Projection::overlaps(Projection p);
	int Projection::overlapsI(Projection p);
};

typedef Projection projection;
