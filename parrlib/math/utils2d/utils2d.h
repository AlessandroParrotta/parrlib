#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

#include "..\Vector2f.h"

#include "AxisAlignedBoundingBox2D.h"
#include "../physics2d/Projection2d.h"

#include "../otherutil.h"

namespace prb {
	namespace utils2 {
		extern float tolerance;
		extern float areaTolerance;

		std::vector<vec2> transform(std::vector<vec2> const& poly, mat3 const& tr);

		struct Line {
			Vector2f p1, p2;

			Line() {}
			Line(Vector2f p1, Vector2f p2) {
				this->p1 = p1;
				this->p2 = p2;
			}
			Line(Vector2f p1, float offset1, Vector2f p2, float offset2) {
				this->p1 = p1 + (p2 - p1).normalized() * offset1;
				this->p2 = p2 + (p1 - p2).normalized() * offset2;
			}
			Line(Vector2f p1, Vector2f p2, float offset) : Line(p1, offset, p2, offset) {}
			Line(Vector2f p1, Vector2f p2, mat3 const& tr) : Line((vec2)(tr*p1), (vec2)(tr*p2)) {}
			Line(std::vector<vec2> const& poly, int id, int dir = 1) : Line(poly[id], poly[dir > 0 ? (id+1)%poly.size() : outl::pmod(id - 1, poly.size())]) {}

			Vector2f dir() const { return (p2 - p1).normalized(); }
			Line inverted() const { return Line(p2, p1); };
			Line offset(float o1, float o2) const { return Line(p1, o1, p2, o2); }
			Line offset(float o) const { return Line(p1, o, p2, o); }
			Vector2f center() const { return p1 + (p2 - p1) / 2.f; }
			Vector2f perc(float p) const { return p1 + (p2 - p1) * p; }
			float dist() const { return (p2 - p1).magnitude(); }

			vec2& operator[](int i) { switch (i) { case 0: return p1; case 1: return p2; default: return p2;} return p2; }
			const vec2& operator[](int i) const { switch (i) { case 0: return p1; case 1: return p2; default: return p2;} return p2; }
			vec2& operator[](unsigned int i) { switch (i) { case 0: return p1; case 1: return p2; default: return p2;} return p2; }
			const vec2& operator[](unsigned int i) const { switch (i) { case 0: return p1; case 1: return p2; default: return p2;} return p2; }

			vec2 operator[](float f) { return perc(f); }
			const vec2 operator[](float f) const { return perc(f); }

			std::vector<vec2> toVec() const { return { p1, p2 }; }
		};
		std::wostream& operator<<(std::wostream& wo, Line const& ln);

		typedef Line line;

		bool pointOnLine(Vector2f point, Line l);

		bool intervalInters(float i1start, float i1end, float i2start, float i2end);

		bool lineInters(Line const& l1, Line const& l2, Vector2f& point);
		bool lineInters(Line const& l1, Line const& l2);

		bool lineIntersOpti(Line const& l1, Line const& l2);

		bool intersLineQuad(Line const& l, aabb2 const& quad);

		Line getPolyEdge(int i, std::vector<Vector2f> const& poly, int dir);
		Line getPolyEdge(int i, std::vector<Vector2f> const& poly);

		int intervalsOverlap(const float& min1, const float& max1, const float& min2, const float& max2);
		bool intervalsOverlapCheck(const float& min1, const float& max1, const float& min2, const float& max2);

		vec2 getCenter(std::vector<vec2> const& poly);

		float calcPerimeter(std::vector<Vector2f>& poly);

		aabb2 calcBoundingBox(std::vector<Vector2f> const& poly);

		float calcArea(std::vector<Vector2f> const& poly, int nIterations);
		float calcArea(std::vector<Vector2f> const& poly);

		struct LinePolyInters {
			int id;
			float dist;
			LinePolyInters() {}
			LinePolyInters(int id, float dist) {
				this->id = id;
				this->dist = dist;
			}
		};
		std::vector<LinePolyInters> lineIntersPoly(Line const& l, std::vector<Vector2f> const& poly, Vector2f const& offset);
		std::vector<LinePolyInters> lineIntersPoly(Line const& l, std::vector<Vector2f> const& poly);

		bool lineIntersPolyCheck(Line const& l, std::vector<Vector2f> const& poly, Vector2f const& offset);
		bool lineIntersPolyCheck(Line const& l, std::vector<Vector2f> const& poly);

		struct PolyPolyInters {
			int idPoly1, idPoly2;
			float distPoly1, distPoly2;

			PolyPolyInters() {}
			PolyPolyInters(int idPoly1, int idPoly2, float distPoly1, float distPoly2) {
				this->idPoly1 = idPoly1;
				this->idPoly2 = idPoly2;
				this->distPoly1 = distPoly1;
				this->distPoly2 = distPoly2;
			}
		};
		std::vector<PolyPolyInters> polyIntersPoly(std::vector<Vector2f> const& poly1, std::vector<Vector2f> const& poly2, Vector2f const& offset);
		std::vector<PolyPolyInters> polyIntersPoly(std::vector<Vector2f> const& poly1, std::vector<Vector2f> const& poly2);

		bool polyIntersPolyCheck(std::vector<Vector2f> const& poly1, std::vector<Vector2f> const& poly2, Vector2f const& offset);
		bool polyIntersPolyCheck(std::vector<Vector2f> const& poly1, std::vector<Vector2f> const& poly2);


		bool contains(Vector2f const& point, std::vector<Vector2f> const& poly, Vector2f const& offset);
		bool contains(Vector2f const& point, std::vector<Vector2f> const& poly);

		bool contains(std::vector<Vector2f> const& container, std::vector<Vector2f> const& poly, Vector2f const& offset);
		bool contains(std::vector<Vector2f> const& container, std::vector<Vector2f> const& poly);

		struct CircleIntersection {
			Vector2f hitPoint;
			float distance;

			Vector2f exitPoint;
			float exitDistance;

			CircleIntersection() {

			}

			CircleIntersection(Vector2f hitPoint, float distance, Vector2f exitPoint, float exitDistance) {
				this->hitPoint = hitPoint;
				this->distance = distance;
				this->exitPoint = exitPoint;
				this->exitDistance = exitDistance;
			}
		};

		bool circleLineInters(Line l, Vector2f cPos, float radius, CircleIntersection& outInters);
		bool circleLineInters(Line l, Vector2f cPos, float radius);

		bool isClockwise(std::vector<Vector2f> const& points);

		std::vector<Vector2f> makeClockwise(std::vector<Vector2f> const& poly);
		std::vector<Vector2f> makeCounterClockwise(std::vector<Vector2f> const& poly);

		Vector2f getEdge(std::vector<Vector2f>& mesh, int i);

		bool isConvex(std::vector<Vector2f>& mesh);

		Vector2f correct(std::vector<Vector2f>& mesh, int id, int dir);
		Vector2f correct(std::vector<Vector2f>& mesh, int id);

		Vector2f correctCW(std::vector<Vector2f>& mesh, int id, int dir = 1);

		std::vector<Vector2f> correctTolerancePoints(std::vector<Vector2f>& poly);

		bool visible(std::vector<Vector2f>& mesh, int v1, int v2);

		std::vector<std::vector<Vector2f>> joinAdjacent(std::vector<std::vector<Vector2f>> polys);

		int absDistCheck(int i, int j, int size);

		struct SelfIntersection {
			int id;
			Vector2f point;

			SelfIntersection() {}
			SelfIntersection(int id, Vector2f point) : id(id), point(point) {}
		};
		SelfIntersection selfIntersects(std::vector<Vector2f>& poly, int idx, bool optimize);
		SelfIntersection selfIntersects(std::vector<Vector2f>& poly, int idx);

		bool collidesWithSelf(std::vector<Vector2f>& poly);
		std::vector<std::vector<Vector2f>> decollide(std::vector<Vector2f>& poly);

		int getDist(int i, int j, int  size, int dir);
		void removeFrom(std::vector<Vector2f>& mesh, int  from, int to, int dir);

		std::vector<std::vector<Vector2f>> makeConvexOld(std::vector<Vector2f> mesh, int startMeshSize, int level);
		std::vector<std::vector<Vector2f>> makeConvex(std::vector<Vector2f> mesh);

		struct BrokenPoly {
			Vector2f offset;
			std::vector<Vector2f> brokenPoly;
			float perimeter = -1.0f, area = -1.0f;
			BrokenPoly() {}
			BrokenPoly(Vector2f offset, std::vector<Vector2f> brokenPoly) {
				this->offset = offset;
				this->brokenPoly = brokenPoly;
			}
			BrokenPoly(Vector2f offset, std::vector<Vector2f> brokenPoly, float perimeter, float area) {
				this->offset = offset;
				this->brokenPoly = brokenPoly;
				this->perimeter = perimeter;
				this->area = area;
			}
		};
		std::vector<BrokenPoly> slicePoly(Line l, std::vector<Vector2f>& poly);

		std::vector<BrokenPoly> breakPoly(Vector2f center, std::vector<Vector2f>& poly);

		std::vector<BrokenPoly> breakPoly(std::vector<Vector2f> destructor, std::vector<Vector2f> poly, int startMeshSize, int level);
		std::vector<BrokenPoly> breakPoly(std::vector<Vector2f> destructor, std::vector<Vector2f> poly);
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace utl2 = utils2;
#endif
}
