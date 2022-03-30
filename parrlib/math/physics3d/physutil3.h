#include "../utils3d/utils3d.h"
#include "../utils3d/axisalignedboundingbox3d.h"

#include "collision3d.h"
#include "collider3d.h"

namespace prb {
	namespace physutil3 {
		std::shared_ptr<Collision3D> checkCollision(std::vector<vec3> const& p1, mat4 const& tr1, std::vector<vec3> const& axes1, std::vector<vec3> const& p2, mat4 const& tr2, std::vector<vec3> const& axes2);
		std::shared_ptr<Collision3D> checkCollision(std::vector<vec3> const& p1, mat4 const& tr1, std::vector<vec3> const& p2, mat4 const& tr2);


		std::shared_ptr<Collision3D> checkCollision(Collider3D* c1, mat4 const& tr1, Collider3D* c2, mat4 const& tr2);

		bool checkCollisionb(aabb3 const& bb1, aabb3 const& bb2);
		std::shared_ptr<Collision3D> checkCollision(aabb3 const& bb1, aabb3 const& bb2);

		std::shared_ptr<Collision3D> checkCollision(utils3::line const& l1, utils3::line const& l2);
		std::shared_ptr<Collision3D> checkCollision(utils3::line const& l, aabb3 const& bb);

		bool checkCollisionb(vec3 const& p, aabb3 const& bb);
		std::shared_ptr<Collision3D> checkCollision(vec3 const& p, aabb3 const& bb);
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace psu3 = physutil3;
#endif
}
