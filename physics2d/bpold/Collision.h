#pragma once

#include <Parrlib\Vector2f.h>

#include "../Utils2D/Consts.h"

class Collider;

class Collision{
public:
	Collider &col1, &col2;
	Vector2f axis;
	float overlap;

	Collision(Collider &col1, Collider &col2, Vector2f axis, float overlap);
	~Collision();

	virtual void solve();
};

typedef Collision collision;
