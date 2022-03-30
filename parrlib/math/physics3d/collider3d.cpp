#include "collider3d.h"


namespace prb {
	Collider3D::Collider3D(std::vector<Vector3f> const& verts) : verts(verts) {
		for (int i = 0; i < verts.size(); i++) {
			axes.push_back((verts[(i + 1) % verts.size()] - verts[i]).normalized().perp());
		}
	}
	Collider3D::Collider3D(std::vector<Vector3f> const& verts, std::vector<vec3> const& axes) : verts(verts), axes(axes) {}


	Collider3D::~Collider3D()
	{
	}

	std::shared_ptr<Collision3D> Collider3D::checkCollision(mat4 const& tr, Collider3D* other, mat4 const& otr) {
		if (!other) return nullptr;

		float overlap = FLT_MAX;
		Vector3f smallestAxis = FLT_MAX;

		for (int i = 0; i < axes.size(); i++) {
			Projection3D p1(*other, tr, axes[i]);
			Projection3D p2(*this, otr, axes[i]);

			if (p1.overlaps(p2)) {
				float ov = p1.getOverlap(p2);

				if (ov != 0.0f && abs(ov) < abs(overlap)) {
					overlap = ov;
					smallestAxis = axes[i];
				}
			}
			else {
				return nullptr;
			}
		}

		for (int i = 0; i < other->axes.size(); i++) {
			Projection3D p1 = Projection3D(*other, tr, other->axes[i]);
			Projection3D p2 = Projection3D(*this, otr, other->axes[i]);

			if (p1.overlaps(p2)) {
				float ov = p1.getOverlap(p2);

				if (ov != 0.0f && abs(ov) < abs(overlap)) {
					overlap = ov;
					smallestAxis = other->axes[i];
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
}
