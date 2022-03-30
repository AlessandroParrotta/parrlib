#include "Collision3d.h"

#include "Collider3d.h"
#include "../utils2d/consts.h"

namespace prb {
	Collision3D::Collision3D(Vector3f const& axis, float const& overlap) :
		axis(axis), overlap(overlap) {}


	Collision3D::~Collision3D()
	{
	}

	Collision3D::solvedcol Collision3D::solve() {
		//float invMass1 = 1.0f / col1->mass;
		//float invMass2 = 1.0f / col2->mass;

		//float relMass = invMass1 / invMass2;

		//col1->pos += axis * (overlap/2.0f + consts::collisionOffset);
		//col2->pos -= axis * (overlap / 2.0f + consts::collisionOffset);

		vec3 val = axis * (overlap / 2.0f + consts::collisionOffset);
		return { val, -val };
	}
}
