#pragma once

#include <vector>

#include <Parrlib\Vector2f.h>

#include "../Utils2D/Utils2D.h"

#include "Collision.h"
#include "Projection.h"

class Collider{
public:
	Vector2f &pos;
	Vector2f oldPos;

	float &rot;
	float oldRot;

	bool collided = false;

	float mass = 1.0f;

	std::vector<Vector2f> &verts;

	std::vector<Vector2f> wVerts;
	std::vector<Vector2f> axes;

	Collider(Vector2f &pos, float &rot, std::vector<Vector2f> &verts);

	~Collider();

	virtual void forceUpdateAxes();
	virtual void updateAxes();

	virtual void forceUpdateWVerts();
	virtual void updateWVerts();

	virtual void update();

	virtual std::shared_ptr<Collision> checkCollision(Collider &other);
};

typedef Collider collider;
