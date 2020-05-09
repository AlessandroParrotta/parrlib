#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

#include <Parrlib\Util.h>
#include <Parrlib\Vector2f.h>
#include <Parrlib\Debug.h>

#include "AxisAlignedBoundingBox2D.h"
#include "../physics2d/Projection.h"

namespace utils2{
	extern float tolerance;
	extern float areaTolerance;

	struct Line {
		Vector2f p1, p2;

		Line(){}
		Line(Vector2f p1, Vector2f p2) {
			this->p1 = p1;
			this->p2 = p2;
		}
		Line(Vector2f p1, float offset1, Vector2f p2, float offset2) {
			this->p1 = p1 + (p2 - p1).normalized()*offset1;
			this->p2 = p2 + (p1 - p2).normalized()*offset2;
		}

		Vector2f dir() const { return (p2 - p1).normalized(); }
		Line inverted() const { return Line(p2, p1); };
		Line offset(float o1, float o2) const { return Line(p1, o1, p2, o2); }
		float dist() const { return (p2 - p1).magnitude(); }
	};

	typedef Line line;

	bool pointOnLine(Vector2f point, Line l);

	bool intervalInters(float i1start, float i1end, float i2start, float i2end);

	bool lineInters(Line const& l1, Line const& l2, Vector2f &point);
	bool lineInters(Line const& l1, Line const& l2);

	bool lineIntersOpti(Line const& l1, Line const& l2);

	bool intersLineQuad(Line const& l, aabb2 const& quad);

	Line getPolyEdge(int i, std::vector<Vector2f> &poly, int dir);
	Line getPolyEdge(int i, std::vector<Vector2f> &poly);
	
	int intervalsOverlap(const float &min1, const float &max1, const float &min2, const float &max2);
	bool intervalsOverlapCheck(const float &min1, const float &max1, const float &min2, const float &max2);
	
	float calcPerimeter(std::vector<Vector2f> &poly);

	aabb2 calcBoundingBox(std::vector<Vector2f> &poly);

	float calcArea(std::vector<Vector2f> &poly, int nIterations);
	float calcArea(std::vector<Vector2f> &poly);

	struct LinePolyInters {
		int id;
		float dist;
		LinePolyInters() {}
		LinePolyInters(int id, float dist) {
			this->id = id;
			this->dist = dist;
		}
	};
	std::vector<LinePolyInters> lineIntersPoly(Line l, std::vector<Vector2f> &poly, Vector2f offset);
	std::vector<LinePolyInters> lineIntersPoly(Line l, std::vector<Vector2f> &poly);

	bool lineIntersPolyCheck(Line l, std::vector<Vector2f> &poly, Vector2f offset);
	bool lineIntersPolyCheck(Line l, std::vector<Vector2f> &poly);
	
	struct PolyPolyInters {
		int idPoly1, idPoly2;
		float distPoly1, distPoly2;

		PolyPolyInters(){}
		PolyPolyInters(int idPoly1, int idPoly2, float distPoly1, float distPoly2) {
			this->idPoly1 = idPoly1;
			this->idPoly2 = idPoly2;
			this->distPoly1 = distPoly1;
			this->distPoly2 = distPoly2;
		}
	};
	std::vector<PolyPolyInters> polyIntersPoly(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2, Vector2f offset);
	std::vector<PolyPolyInters> polyIntersPoly(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2);
	
	bool polyIntersPolyCheck(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2, Vector2f offset);
	bool polyIntersPolyCheck(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2);


	bool contains(Vector2f point, std::vector<Vector2f> &poly, Vector2f offset);
	bool contains(Vector2f point, std::vector<Vector2f> &poly);

	bool contains(std::vector<Vector2f> &container, std::vector<Vector2f> &poly, Vector2f offset);
	bool contains(std::vector<Vector2f> &container, std::vector<Vector2f> &poly);

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

	bool circleLineInters(Line l, Vector2f cPos, float radius, CircleIntersection &outInters);
	bool circleLineInters(Line l, Vector2f cPos, float radius);

	bool isClockwise(std::vector<Vector2f> &points);

	std::vector<Vector2f> makeClockwise(std::vector<Vector2f> &poly);
	std::vector<Vector2f> makeCounterClockwise(std::vector<Vector2f> &poly);

	Vector2f getEdge(std::vector<Vector2f> &mesh, int i);

	bool isConvex(std::vector<Vector2f> &mesh);

	Vector2f correct(std::vector<Vector2f> &mesh, int id, int dir);
	Vector2f correct(std::vector<Vector2f> &mesh, int id);

	std::vector<Vector2f> correctTolerancePoints(std::vector<Vector2f> &poly);

	bool visible(std::vector<Vector2f> &mesh, int v1, int v2);

	std::vector<std::vector<Vector2f>> joinAdjacent(std::vector<std::vector<Vector2f>> polys);

	int absDistCheck(int i, int j, int size);

	struct SelfIntersection {
		int id;
		Vector2f point;

		SelfIntersection() {}
		SelfIntersection(int id, Vector2f point) : id(id), point(point){}
	};
	SelfIntersection selfIntersects(std::vector<Vector2f> &poly, int idx, bool optimize);
	SelfIntersection selfIntersects(std::vector<Vector2f> &poly, int idx);

	bool collidesWithSelf(std::vector<Vector2f> &poly);
	std::vector<std::vector<Vector2f>> decollide(std::vector<Vector2f> &poly);

	int getDist(int i, int j, int  size, int dir);
	void removeFrom(std::vector<Vector2f> &mesh, int  from, int to, int dir);

	std::vector<std::vector<Vector2f>> makeConvexOld(std::vector<Vector2f> mesh, int startMeshSize, int level);
	std::vector<std::vector<Vector2f>> makeConvex(std::vector<Vector2f> mesh);

	struct BrokenPoly {
		Vector2f offset;
		std::vector<Vector2f> brokenPoly;
		float perimeter=-1.0f, area=-1.0f;
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
	std::vector<BrokenPoly> slicePoly(Line l, std::vector<Vector2f> &poly);

	std::vector<BrokenPoly> breakPoly(Vector2f center, std::vector<Vector2f> &poly);

	std::vector<BrokenPoly> breakPoly(std::vector<Vector2f> destructor, std::vector<Vector2f> poly, int startMeshSize, int level);
	std::vector<BrokenPoly> breakPoly(std::vector<Vector2f> destructor, std::vector<Vector2f> poly);
}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace utl2 = utils2;
#endif
