#include "Collision2d.h"

#include "../Utils2D/Utils2D.h"
#include "Collider2d.h"

namespace prb {
	namespace physutil2 {
		std::shared_ptr<Collision2D> checkCollision(std::vector<vec2> const& p1, mat3 const& tr1, std::vector<vec2> const& axes1, std::vector<vec2> const& p2, mat3 const& tr2, std::vector<vec2> const& axes2);
		std::shared_ptr<Collision2D> checkCollision(std::vector<vec2> const& p1, mat3 const& tr1, std::vector<vec2> const& p2, mat3 const& tr2);


		std::shared_ptr<Collision2D> checkCollision(Collider2D* c1, mat3 const& tr1, Collider2D* c2, mat3 const& tr2);

		bool checkCollisionb(aabb2 const& bb1, aabb2 const& bb2);
		std::shared_ptr<Collision2D> checkCollision(aabb2 const& bb1, aabb2 const& bb2);

		std::shared_ptr<Collision2D> checkCollision(utils2::line const& l1, utils2::line const& l2);
		std::shared_ptr<Collision2D> checkCollision(utils2::line const& l, aabb2 const& bb);

		bool checkCollisionb(vec2 const& p, aabb2 const& bb);
		std::shared_ptr<Collision2D> checkCollision(vec2 const& p, aabb2 const& bb);
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace psu2 = physutil2;
#endif
}
