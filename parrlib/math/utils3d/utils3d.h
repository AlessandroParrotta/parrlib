#pragma once

#include <vector>

#include "../vector3f.h"
#include "../matrix4f.h"
#include "../utils3d/axisalignedboundingbox3d.h"

namespace prb{
	namespace utils3 {
		struct Line {
			Vector3f p1, p2;

			Line() {}
			Line(Vector3f p1, Vector3f p2) {
				this->p1 = p1;
				this->p2 = p2;
			}
			Line(Vector3f p1, float offset1, Vector3f p2, float offset2) {
				this->p1 = p1 + (p2 - p1).normalized() * offset1;
				this->p2 = p2 + (p1 - p2).normalized() * offset2;
			}

			Vector3f dir() const { return (p2 - p1).normalized(); }
			Line inverted() const { return Line(p2, p1); };
			Line offset(float o1, float o2) const { return Line(p1, o1, p2, o2); }
			Vector3f center() const { return (p2 - p1) / 2.f; }
			Vector3f perc(float p) const { return p1 + (p2 - p1) * p; }
			float dist() const { return (p2 - p1).magnitude(); }

			vec3& operator[](unsigned int i) {
				switch (i) {
				case 0: return p1;
				case 1: return p2;
				default: return p2;
				}
				return p2;
			}

			vec3 operator[](unsigned int i) const {
				switch (i) {
				case 0: return p1;
				case 1: return p2;
				default: return p2;
				}
				return p2;
			}

			std::vector<vec3> toVec() const { return { p1, p2 }; }
		};
		typedef Line line;

		struct OrientedBoundingBox3D {
			aabb3 bb;
			mat4 tr;
		};
		typedef OrientedBoundingBox3D obb3;
	}
}
