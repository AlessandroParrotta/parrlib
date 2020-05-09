#include <parrlib/physics2d/collider.h>



Collider::Collider(std::vector<Vector2f> const& verts) : verts(verts){
	for (int i = 0; i < verts.size(); i++) {
		axes.push_back((verts[(i+1)%verts.size()] - verts[i]).normalized().perp());
	}
}
Collider::Collider(std::vector<Vector2f> const& verts, std::vector<vec2> const& axes) : verts(verts), axes(axes) {}


Collider::~Collider()
{
}

std::shared_ptr<Collision> Collider::checkCollision(mat3 const& tr, Collider* other, mat3 const& otr) {
	if (!other) return nullptr;

	float overlap = FLT_MAX;
	Vector2f smallestAxis = FLT_MAX;

	for (int i = 0; i < axes.size(); i++) {
		Projection p1(*other, tr, axes[i]);
		Projection p2(*this, otr, axes[i]);

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
		Projection p1 = Projection(*other, tr, other->axes[i]);
		Projection p2 = Projection(*this, otr, other->axes[i]);

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
	return std::make_shared<Collision>(smallestAxis, overlap);
}
