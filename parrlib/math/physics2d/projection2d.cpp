#include "Projection2d.h"

#include "Collider2d.h"

namespace prb {
	Projection2D::Projection2D(float min, float max) {
		//this->min = min;
		//this->max = max;

		this->min = std::fmin(min, max);
		this->max = std::fmax(min, max);
	}

	Projection2D::Projection2D(std::vector<vec2> const& vs, mat3 const& tr, Vector2f const& axis) {
		if (vs.size() <= 0) return;

		min = axis.dot(tr * vs[0]);

		max = min;
		for (int i = 1; i < vs.size(); i++) {
			float proj = axis.dot(tr * vs[i]);

			if (proj > max) max = proj;
			if (proj < min) min = proj;
		}
	}

	Projection2D::Projection2D(vec2 const& v1, vec2 const& v2, Vector2f const& axis) {
		min = max = axis.dot(v1);

		float d = axis.dot(v2);
		if (d > max) max = d;
		else if (d < min) min = d;
	}

	Projection2D::Projection2D(aabb2 const& bb, vec2 const& axis) {
		min = max = axis.dot(bb.get(0));

		float d = axis.dot(bb.get(1));
		if (d > max) max = d;
		else if (d < min) min = d;

		d = axis.dot(bb.get(2));
		if (d > max) max = d;
		else if (d < min) min = d;

		d = axis.dot(bb.get(3));
		if (d > max) max = d;
		else if (d < min) min = d;
	}

	Projection2D::Projection2D(Collider2D const& col, mat3 const& tr, Vector2f const& axis) : Projection2D(col.verts, tr, axis) {}


	Projection2D::~Projection2D()
	{
	}

	float Projection2D::getOverlap(Projection2D p) {
		int over = overlapsI(p);
		if (over == 0) return max - p.min;
		else if (over == 1) return min - p.max;
		else {
			float c1 = min - p.max, c2 = max - p.min;
			float ret = abs(c1) < abs(c2) ? c1 : c2;
			return ret;
		}
	}

	bool Projection2D::overlaps(Projection2D p) {
		return !(max < p.min || min > p.max);
	}

	int Projection2D::overlapsI(Projection2D p) {
		//you can optimize this i think

		//might not work
		//here i'm assuming min is always smaller than max and max is always bigger than min (as it should be)
		if (max < p.min || min > p.max) return -1;

		if (min < p.min && max < p.max && max >= p.min) {
			return 0;
		}
		else if (max > p.max && min > p.min && min <= p.max) {
			return 1;
		}
		else if (min < p.min && max > p.max) {
			return 2;
		}
		else  if (min >= p.min && max <= p.max) {
			return 3;
		}

		return -1;
	}
}
