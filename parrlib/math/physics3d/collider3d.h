#pragma once

#include <vector>

#include "../Vector3f.h"
#include "../matrix4f.h"

#include "../utils3d/utils3d.h"

#include "collision3d.h"
#include "projection3d.h"

namespace prb {
	class Collider3D {
	public:
		float mass = 1.0f;

		std::vector<vec3> verts;
		std::vector<vec3> axes;

		Collider3D(std::vector<Vector3f> const& verts);
		Collider3D(std::vector<Vector3f> const& verts, std::vector<vec3> const& axes);

		~Collider3D();

		virtual std::shared_ptr<Collision3D> checkCollision(mat4 const& tr, Collider3D* other, mat4 const& otr);
	};

	typedef Collider3D col3;
}
