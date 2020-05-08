#pragma once

#include <vector>

#include <Parrlib\Vector2f.h>

#include "../Utils2D/Utils2D.h"

#include "Collision.h"
#include "Projection.h"

class Collider{
public:
	float mass = 1.0f;

	std::vector<Vector2f> verts;
	std::vector<vec2> axes;

	Collider(std::vector<Vector2f> const& verts);
	Collider(std::vector<Vector2f> const& verts, std::vector<vec2> const& axes);

	~Collider();

	virtual std::shared_ptr<Collision> checkCollision(mat3 const& tr, Collider *other, mat3 const& otr);
};

typedef Collider collider;
