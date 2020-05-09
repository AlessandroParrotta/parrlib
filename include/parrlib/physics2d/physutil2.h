#include "Collision.h"

#include "../Utils2D/Utils2D.h"
#include "Collider.h"

namespace physutil2 {
	std::shared_ptr<Collision> checkCollision(std::vector<vec2> const& p1, mat3 const& tr1, std::vector<vec2> const& axes1, std::vector<vec2> const& p2, mat3 const& tr2, std::vector<vec2> const& axes2);
	std::shared_ptr<Collision> checkCollision(std::vector<vec2> const& p1, mat3 const& tr1, std::vector<vec2> const& p2, mat3 const& tr2);


	std::shared_ptr<Collision> checkCollision(Collider* c1, mat3 const& tr1, Collider* c2, mat3 const& tr2);
	
	bool checkCollisionb(aabb2 const& bb1, aabb2 const& bb2);
	std::shared_ptr<Collision> checkCollision(aabb2 const& bb1, aabb2 const& bb2);

	std::shared_ptr<Collision> checkCollision(utils2::line const& l1, utils2::line const& l2);
	std::shared_ptr<Collision> checkCollision(utils2::line const& l, aabb2 const& bb);

	bool checkCollisionb(vec2 const& p, aabb2 const& bb);
	std::shared_ptr<Collision> checkCollision(vec2 const& p, aabb2 const& bb);
}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace psu2 = physutil2;
#endif
