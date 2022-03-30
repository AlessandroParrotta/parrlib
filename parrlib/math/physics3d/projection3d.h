#pragma once

#include <vector>

#include "../Vector3f.h"
#include "../Utils3D/AxisAlignedBoundingBox3D.h"
#include "../Matrix4f.h"

namespace prb {
	class Collider3D;

	class Projection3D {
	public:
		float min, max;

		Projection3D(float min, float max);
		Projection3D(std::vector<vec3> const& vs, mat4 const& tr, Vector3f const& axis);
		Projection3D(vec3 const& v1, vec3 const& v2, Vector3f const& axis);
		Projection3D(aabb3 const& bb, vec3 const& axis);
		Projection3D(Collider3D const& col, mat4 const& tr, Vector3f const& axis);

		~Projection3D();

		float getOverlap(Projection3D p);
		bool overlaps(Projection3D p);
		int overlapsI(Projection3D p);
	};

	typedef Projection3D proj3;
}
