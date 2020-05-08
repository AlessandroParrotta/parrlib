#include "Collision.h"

#include "Collider.h"

Collision::Collision(Collider &col1, Collider &col2, Vector2f axis, float overlap) :
	col1(col1), col2(col2), axis(axis), overlap(overlap){}


Collision::~Collision()
{
}

void Collision::solve() {
	float invMass1 = 1.0f / col1.mass;
	float invMass2 = 1.0f / col2.mass;

	float relMass = invMass1 / invMass2;

#ifdef COLLIDER_DEBUG
	Debug::drawLine(Consts::project(col1.pos), Consts::project(col1.pos-axis*overlap), Vector4f(1.0f,0.0f,0.0f,1.0f));
#endif
	
	col1.pos += axis * (overlap/2.0f + consts::collisionOffset);
	col2.pos -= axis * (overlap / 2.0f + consts::collisionOffset);

#ifdef COLLIDER_DEBUG
	Debug::rtlog << "solving " << axis.toString() << " " << overlap << "\n";
#endif
}
