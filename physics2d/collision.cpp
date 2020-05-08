#include "Collision.h"

#include "Collider.h"

Collision::Collision(Vector2f const& axis, float const& overlap) :
	axis(axis), overlap(overlap){}


Collision::~Collision()
{
}

Collision::solvedcol Collision::solve() {
	//float invMass1 = 1.0f / col1->mass;
	//float invMass2 = 1.0f / col2->mass;

	//float relMass = invMass1 / invMass2;
	
	//col1->pos += axis * (overlap/2.0f + consts::collisionOffset);
	//col2->pos -= axis * (overlap / 2.0f + consts::collisionOffset);

	vec2 val = axis * (overlap / 2.0f + consts::collisionOffset);
	return { val, -val };
}
