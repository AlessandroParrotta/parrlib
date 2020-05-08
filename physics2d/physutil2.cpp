#include "physutil2.h"

namespace physutil2 {
	std::shared_ptr<Collision> checkCollision(std::vector<vec2> const& ps1, mat3 const& tr1, std::vector<vec2> const& axes1, std::vector<vec2> const& ps2, mat3 const& tr2, std::vector<vec2> const& axes2) {
		float overlap = FLT_MAX;
		Vector2f smallestAxis = FLT_MAX;

		for (int i = 0; i < axes1.size(); i++) {
			Projection p1(ps2, tr2, axes1[i]);
			Projection p2(ps1, tr1, axes1[i]);

			if (p1.overlaps(p2)) {
				float ov = p1.getOverlap(p2);

				if (ov != 0.0f && abs(ov) < abs(overlap)) {
					overlap = ov;
					smallestAxis = axes1[i];
				}
			}
			else {
				return nullptr;
			}
		}

		for (int i = 0; i < axes2.size(); i++) {
			Projection p1 = Projection(ps2, tr2, axes2[i]);
			Projection p2 = Projection(ps1, tr1, axes2[i]);

			if (p1.overlaps(p2)) {
				float ov = p1.getOverlap(p2);

				if (ov != 0.0f && abs(ov) < abs(overlap)) {
					overlap = ov;
					smallestAxis = axes2[i];
				}
			}
			else {
				return nullptr;
			}
		}

		if (overlap == FLT_MAX || overlap == 0.0f) return nullptr;

		//return std::make_shared<Collision>(this, tr, other, otr, smallestAxis, overlap);
		return std::make_shared<Collision>(smallestAxis, overlap);
	}

	std::shared_ptr<Collision> checkCollision(std::vector<vec2> const& p1, mat3 const& tr1, std::vector<vec2> const& p2, mat3 const& tr2) {
		std::vector<vec2> axes1; mat3 rotm1 = pmat3::getRotation(tr1); for (int i = 0; i < p1.size(); i++) axes1.push_back(rotm1 * (p1[(i+1)%p1.size()] - p1[i]).normalized().perp());
		std::vector<vec2> axes2; mat3 rotm2 = pmat3::getRotation(tr2); for (int i = 0; i < p2.size(); i++) axes2.push_back(rotm2 * (p2[(i+1)%p1.size()] - p2[i]).normalized().perp());
		return checkCollision(p1, tr1, axes1, p2, tr2, axes2);
	}

	std::shared_ptr<Collision> checkCollision(Collider* c1, mat3 const& tr1, Collider* c2, mat3 const& tr2) {
		return c1->checkCollision(tr1, c2, tr2);
	}

	bool checkCollisionb(aabb2 const& bb1, aabb2 const& bb2) {
		return	!(bb1.maxx() < bb2.minx() || bb1.maxy() < bb2.miny() ||
				  bb2.maxx() < bb1.minx() || bb2.maxy() < bb1.miny());
	}

	std::shared_ptr<Collision> checkCollision(aabb2 const& bb1, aabb2 const& bb2) {
		if (!checkCollisionb(bb1, bb2)) return nullptr;

		projection p0(bb1.minx(), bb1.maxx());
		projection p1(bb2.minx(), bb2.maxx());

		projection p2(bb1.miny(), bb1.maxy());
		projection p3(bb2.miny(), bb2.maxy());

		float ov0 = p0.getOverlap(p1);
		float ov1 = p2.getOverlap(p3);

		float minov = abs(ov0) < abs(ov1) ? ov0 : ov1;
		vec2 smallestax = abs(ov0) < abs(ov1) ? v2ax::fwv2 : v2ax::upv2;

		return std::make_shared<collision>(smallestax, minov);
	}

	std::shared_ptr<Collision> checkCollision(utils2::line const& l1, utils2::line const& l2) {
		vec2 ax0 = l1.dir().perp();
		float selfproj0 = ax0.dot(l1.p1);
		Projection p0(l2.p1, l2.p2, ax0);
		if (selfproj0 < p0.min || selfproj0 > p0.max) return nullptr;

		vec2 ax1 = l2.dir().perp();
		float selfproj1 = ax1.dot(l2.p1);
		projection p1(l1.p1, l1.p2, ax1);
		if (selfproj1 < p1.min || selfproj1 > p1.max) return nullptr;

		float ov0 = std::fmin(selfproj0 - p0.min, p0.max - selfproj0);
		float ov1 = std::fmin(selfproj1 - p1.min, p1.max - selfproj1);

		float minov = abs(ov0) < abs(ov1) ? ov0 : ov1;
		vec2 smallestax = abs(ov0) < abs(ov1) ? ax0 : ax1;

		return std::make_shared<Collision>(smallestax, minov);
	}

	std::shared_ptr<Collision> checkCollision(utils2::line const& l, aabb2 const& bb) {
		vec2 ax0 = l.dir().perp();
		float selfproj = ax0.dot(l.p1);
		Projection p0(bb, ax0);
		if (selfproj < p0.min || selfproj > p0.max) return nullptr;

		vec2 ax1 = v2ax::upv2;
		Projection p1(bb, ax1);
		Projection p2(l.p1, l.p2, ax1);
		if (!p1.overlaps(p2)) return nullptr;

		vec2 ax2 = v2ax::fwv2;
		Projection p3 = { bb, ax2 };
		Projection p4 = { l.p1, l.p2, ax2 };
		if (!p3.overlaps(p4)) return nullptr;

		float self1 = p0.min - selfproj, self2 = p0.max - selfproj;

		float ov0 = abs(self1) < abs(self2) ? self1 : self2;
		float ov1 = p1.getOverlap(p2);
		float ov2 = p3.getOverlap(p4);

		//std::cout << "ov0 " << ov0 << "\nov1 " << ov1 << "\nov2 " << ov2 << "\n-\n";

		float minov = (abs(ov0) < abs(ov1) ? abs(ov0) : abs(ov1)) < abs(ov2) ? (abs(ov0) < abs(ov1) ? ov0 : ov1) : ov2;
		vec2 smallestax = minov == ov0 ? ax0 : minov == ov1 ? ax1 : minov == ov2 ? ax2 : ax0;

		//std::cout << "ax " << smallestax << "\n-\n";

		return std::make_shared<Collision>(smallestax, minov);
	}

	bool checkCollisionb(vec2 const& p, aabb2 const& bb) {
		return p >= bb.fmin() && p <= bb.fmax();
	}

	std::shared_ptr<Collision> checkCollision(vec2 const& p, aabb2 const& bb) {
		if (!checkCollisionb(bb, p)) return nullptr;

		float minovx = outl::minfabs(bb.maxx() - p.x, bb.minx() - p.x);
		float minovy = outl::minfabs(bb.maxy() - p.y, bb.miny() - p.y);
		int cmp = std::fabs(minovx) < std::fabs(minovy);
		return std::make_shared<Collision>(cmp ? vec2x(1.f) : vec2y(1.f), cmp ? minovx : minovy);
	}
}
