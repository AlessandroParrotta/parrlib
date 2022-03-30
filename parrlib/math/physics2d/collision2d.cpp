#include "Collision2d.h"

#include "Collider2d.h"

namespace prb {
	Collision2D::Collision2D(Vector2f const& axis, float const& overlap) :
		axis(axis), overlap(overlap) {}


	Collision2D::~Collision2D()
	{
	}

	Collision2D::solvedcol Collision2D::solve() {
		//float invMass1 = 1.0f / col1->mass;
		//float invMass2 = 1.0f / col2->mass;

		//float relMass = invMass1 / invMass2;

		//col1->pos += axis * (overlap/2.0f + consts::collisionOffset);
		//col2->pos -= axis * (overlap / 2.0f + consts::collisionOffset);

		vec2 val = axis * (overlap / 2.0f + consts::collisionOffset);
		return { val, -val };
	}
}
