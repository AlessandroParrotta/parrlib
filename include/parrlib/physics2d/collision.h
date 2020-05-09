#pragma once

#include <Parrlib\Vector2f.h>

#include "../Utils2D/Consts.h"
#include "../Matrix3f.h"

class Collider;

class Collision{
public:
	Vector2f axis;
	float overlap;

	Collision(Vector2f const& axis, float const& overlap);
	~Collision();

	struct solvedcol { vec2 correction1, correction2; };
	virtual solvedcol solve();
};

typedef Collision collision;
