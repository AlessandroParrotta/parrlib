#include "physutil3.h"

#include "../otherutil.h"

#include "../utils3d/axisalignedboundingbox3d.h"
#include "../utils3d/utils3d.h"
#include "../../debug.h"

namespace prb {
	namespace physutil3 {
		std::shared_ptr<Collision3D> checkCollisionPoly(std::vector<vec3> const& ps1, mat4 const& tr1, std::vector<vec3> const& axes1, std::vector<vec3> const& ps2, mat4 const& tr2, std::vector<vec3> const& axes2) {
			float overlap = FLT_MAX;
			Vector3f smallestAxis = FLT_MAX;

			for (int i = 0; i < axes1.size(); i++) {
				Projection3D p1(ps2, tr2, axes1[i]);
				Projection3D p2(ps1, tr1, axes1[i]);

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
				Projection3D p1 = Projection3D(ps2, tr2, axes2[i]);
				Projection3D p2 = Projection3D(ps1, tr1, axes2[i]);

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
			return std::make_shared<Collision3D>(smallestAxis, overlap);
		}

		std::shared_ptr<Collision3D> checkCollision(std::vector<vec3> const& ps1, mat4 const& tr1, std::vector<vec3> const& axes1, std::vector<vec3> const& ps2, mat4 const& tr2, std::vector<vec3> const& axes2) {
			float overlap = FLT_MAX;
			Vector3f smallestAxis = FLT_MAX;

			for (int i = 0; i < ps1.size(); i += 3) {
				if (i + 3 > ps1.size()) break;

				for (int j = 0; j < ps2.size(); j += 3) {
					if (j + 3 > ps2.size()) break;

					deb::tss << i << "/" << ps1.size() << " " << j << "/" << ps2.size() << "\n"; deb::outStr();

					std::shared_ptr<Collision3D> col = checkCollisionPoly(
						{ ps1[i], ps1[i + 1], ps1[i + 2] }, tr1, { axes1[i], axes1[i + 1], axes1[i + 2] },
						{ ps2[j], ps2[j + 1], ps2[j + 2] }, tr2, { axes2[j], axes2[j + 1], axes2[j + 2] }
					);

					if (col) {
						if (col->overlap < overlap) { overlap = col->overlap; smallestAxis = col->axis; }
					}
				}
			}

			return std::make_shared<Collision3D>(smallestAxis, overlap);
		}

		std::shared_ptr<Collision3D> checkCollision(std::vector<vec3> const& p1, mat4 const& tr1, std::vector<vec3> const& p2, mat4 const& tr2) {
			std::vector<vec3> axes1; mat4 rotm1 = tr1.rotation(); for (int i = 0; i < p1.size(); i++) axes1.push_back(rotm1 * (p1[(i + 1) % p1.size()] - p1[i]).normalized().perp((p1[(i+1)%p1.size()]- p1[i]).normalized()));
			std::vector<vec3> axes2; mat4 rotm2 = tr2.rotation(); for (int i = 0; i < p2.size(); i++) axes2.push_back(rotm2 * (p2[(i + 1) % p1.size()] - p2[i]).normalized().perp((p2[(i+1)%p2.size()] - p2[i]).normalized()));
			return checkCollision(p1, tr1, axes1, p2, tr2, axes2);
		}

		std::shared_ptr<Collision3D> checkCollision(Collider3D* c1, mat4 const& tr1, Collider3D* c2, mat4 const& tr2) {
			return c1->checkCollision(tr1, c2, tr2);
		}

		bool checkCollisionb(aabb3 const& bb1, aabb3 const& bb2) {
			return	!(
				bb1.maxx() < bb2.minx() || bb2.maxx() < bb1.minx() || 
				bb1.maxy() < bb2.miny() || bb2.maxy() < bb1.miny() || 
				bb1.maxz() < bb2.minz() || bb2.maxz() < bb1.minz()
			);
		}

		std::shared_ptr<Collision3D> checkCollision(aabb3 const& bb1, aabb3 const& bb2) {
			if (!checkCollisionb(bb1, bb2)) return nullptr;

			proj3 p0(bb1.minx(), bb1.maxx());
			proj3 p1(bb2.minx(), bb2.maxx());

			proj3 p2(bb1.miny(), bb1.maxy());
			proj3 p3(bb2.miny(), bb2.maxy());

			proj3 p4(bb1.minz(), bb1.maxz());
			proj3 p5(bb2.minz(), bb2.maxz());

			float ov0 = p0.getOverlap(p1);
			float ov1 = p2.getOverlap(p3);
			float ov2 = p4.getOverlap(p5);

			//float minov = abs(ov0) < abs(ov1) ? abs(ov2) < abs(ov0) ? ov2 : ov0 : abs(ov2) < abs(ov1) ? ov2 : ov1;
			//vec3 smallestax = abs(ov0) < abs(ov1) ? abs(ov2) < abs(ov0) ? v3ax::fwv3 : v3ax::riv3 : abs(ov2) < abs(ov1) ? v3ax::fwv3 : v3ax::upv3;

			float minov = outl::smallestCheck(ov0, ov1, ov2);
			vec3 smallestax = outl::smallestCheck(ov0, v3ax::riv3, ov1, v3ax::upv3, ov2, v3ax::fwv3);

			return std::make_shared<coll3>(smallestax, minov);
		}

		std::shared_ptr<Collision3D> checkCollision(utils3::line const& l1, utils3::line const& l2) {
			vec3 ax0 = l1.dir().perp();
			float selfproj0 = ax0.dot(l1.p1);
			Projection3D p0(l2.p1, l2.p2, ax0);
			if (selfproj0 < p0.min || selfproj0 > p0.max) return nullptr;

			vec3 ax1 = l2.dir().perp();
			float selfproj1 = ax1.dot(l2.p1);
			proj3 p1(l1.p1, l1.p2, ax1);
			if (selfproj1 < p1.min || selfproj1 > p1.max) return nullptr;

			float ov0 = std::fmin(selfproj0 - p0.min, p0.max - selfproj0);
			float ov1 = std::fmin(selfproj1 - p1.min, p1.max - selfproj1);

			float minov = abs(ov0) < abs(ov1) ? ov0 : ov1;
			vec3 smallestax = abs(ov0) < abs(ov1) ? ax0 : ax1;

			return std::make_shared<Collision3D>(smallestax, minov);
		}

		std::shared_ptr<Collision3D> checkCollision(utils3::line const& l, aabb3 const& bb) {
			vec3 ax0 = l.dir().perp();
			float selfproj = ax0.dot(l.p1);
			Projection3D p0(bb, ax0);
			if (selfproj < p0.min || selfproj > p0.max) return nullptr;

			vec3 ax1 = v3ax::upv3;
			Projection3D p1(bb, ax1);
			Projection3D p2(l.p1, l.p2, ax1);
			if (!p1.overlaps(p2)) return nullptr;

			vec3 ax2 = v3ax::fwv3;
			Projection3D p3 = { bb, ax2 };
			Projection3D p4 = { l.p1, l.p2, ax2 };
			if (!p3.overlaps(p4)) return nullptr;

			float self1 = p0.min - selfproj, self2 = p0.max - selfproj;

			float ov0 = abs(self1) < abs(self2) ? self1 : self2;
			float ov1 = p1.getOverlap(p2);
			float ov2 = p3.getOverlap(p4);

			//std::cout << "ov0 " << ov0 << "\nov1 " << ov1 << "\nov2 " << ov2 << "\n-\n";

			float minov = outl::smallestCheck(ov0, ov1, ov2);
			vec3 smallestax = outl::smallestCheck(ov0, ax0, ov1, ax1, ov2, ax2);

			//std::cout << "ax " << smallestax << "\n-\n";

			return std::make_shared<Collision3D>(smallestax, minov);
		}

		bool checkCollisionb(vec3 const& p, aabb3 const& bb) {
			return p >= bb.fmin() && p <= bb.fmax();
		}

		std::shared_ptr<Collision3D> checkCollision(vec3 const& p, aabb3 const& bb) {
			if (!checkCollisionb(p, bb)) return nullptr;

			float minovx = outl::minfabs(bb.maxx() - p.x, bb.minx() - p.x);
			float minovy = outl::minfabs(bb.maxy() - p.y, bb.miny() - p.y);
			float minovz = outl::minfabs(bb.maxz() - p.z, bb.minz() - p.z);

			return std::make_shared<Collision3D>(outl::smallestCheck(minovx, v3ax::riv3, minovy, v3ax::upv3, minovz, v3ax::fwv3), outl::smallestCheck(minovx, minovy, minovz));
		}
	}
}
