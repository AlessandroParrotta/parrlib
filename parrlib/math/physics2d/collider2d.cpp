#include "collider2d.h"


namespace prb {
	Collider2D::Collider2D(std::vector<Vector2f> const& verts) : verts(verts) {
		for (int i = 0; i < verts.size(); i++) {
			axes.push_back((verts[(i + 1) % verts.size()] - verts[i]).normalized().perp());
		}
	}
	Collider2D::Collider2D(std::vector<Vector2f> const& verts, std::vector<vec2> const& axes) : verts(verts), axes(axes) {}


	Collider2D::~Collider2D()
	{
	}

	std::shared_ptr<Collision2D> Collider2D::checkCollision(mat3 const& tr, Collider2D* other, mat3 const& otr) {
		if (!other) return nullptr;

		float overlap = FLT_MAX;
		Vector2f smallestAxis = FLT_MAX;

		for (int i = 0; i < axes.size(); i++) {
			Projection2D p1(*other, tr, axes[i]);
			Projection2D p2(*this, otr, axes[i]);

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
			Projection2D p1 = Projection2D(*other, tr, other->axes[i]);
			Projection2D p2 = Projection2D(*this, otr, other->axes[i]);

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
		return std::make_shared<Collision2D>(smallestAxis, overlap);
	}
}
