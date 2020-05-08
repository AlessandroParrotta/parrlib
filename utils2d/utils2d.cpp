#include "Utils2D.h"

namespace utils2 {
	float tolerance = 0.00001f;
	float areaTolerance = tolerance / 16.0f;

	bool pointOnLine(Vector2f point, Line l) {
		float lDist = (l.p2 - l.p1).magnitude();

		float l1PDist = (point - l.p1).magnitude();
		float l2PDist = (l.p2 - point).magnitude();
		float lPDistSum = l1PDist + l2PDist;

		return lPDistSum < lDist + tolerance &&
			lPDistSum > lDist - tolerance;
	}

	bool intervalInters(float i1start, float i1end, float i2start, float i2end) {
		return  (i2start <= i1start && i2end >= i1start) || (i2start <= i1end && i2end >= i1end) ||
			(i2start >= i1start && i2end <= i1end) || (i2start <= i1start && i2end >= i1end);
	}

	bool lineInters(Line const& l1, Line const& l2, Vector2f &point) {
		Vector2f l1d = l1.dir(), l2d = l2.dir();
		if (l1d.equalsRange(l2d, tolerance) || l1d.equalsRange(-l2d, tolerance)) {
			//std::cout << "lines have the same dir\n";
			Line ld = Line(l1.p1, l2.p1);
			if (ld.dir().equalsRange(l1d, tolerance) || ld.dir().equalsRange(l2d, tolerance)) {
				//std::cout << "lines are aligned\n";

				Vector2f axis = l1d;

				Vector2f rl1p2 = l1.p2 - l1.p1;
				Vector2f rl2p1 = l2.p1 - l1.p1;
				Vector2f rl2p2 = l2.p2 - l1.p1;

				float dl1p2 = rl1p2.dot(axis);
				float dl2p1 = rl2p1.dot(axis);
				float dl2p2 = rl2p2.dot(axis);

				/*float dl1p2 = axis.dot(rl1p2);
				float dl2p1 = axis.dot(rl2p1);
				float dl2p2 = axis.dot(rl2p2);*/

				if (intervalInters(0.0f, dl1p2, dl2p1, dl2p2)) {
					//std::cout << "lines are interesecting\n";
					point = l2.p1;
					return true;
				}
			}

		}

		// Line AB represented as a1x + b1y = c1
		float a1 = l1.p2.y - l1.p1.y;
		float b1 = l1.p1.x - l1.p2.x;
		float c1 = a1 * (l1.p1.x) + b1 * (l1.p1.y);

		// Line CD represented as a2x + b2y = c2
		float a2 = l2.p2.y - l2.p1.y;
		float b2 = l2.p1.x - l2.p2.x;
		float c2 = a2 * (l2.p1.x) + b2 * (l2.p1.y);

		float determinant = a1 * b2 - a2 * b1;

		if (determinant == 0.0f)
		{
			return false;
		}
		else
		{
			float x = (b2*c1 - b1 * c2) / determinant;
			float y = (a1*c2 - a2 * c1) / determinant;
			Vector2f p = Vector2f(x, y);

			if (((p - l1.p1).magnitude() > (l1.p2 - l1.p1).magnitude() ||
				(p - l2.p1).magnitude() > (l2.p2 - l2.p1).magnitude())) {

				return false;
			}
			else {
				if (((l1.p2 - l1.p1).dot(p - l1.p1) > 0.0f && (l2.p2 - l2.p1).dot(p - l2.p1) > 0.0f)) {
					point = p;
					return true;
				}
				else {
					return false;
				}
			}
		}

	}

	bool lineInters(Line const& l1, Line const& l2) {
		Vector2f point;
		return lineInters(l1, l2, point);
	}

	bool lineIntersOpti(Line const& l1, Line const& l2) {
		vec2 ax0 = l1.dir().perp();
		float selfproj = ax0.dot(l1.p1);
		Projection p0(l2.p1, l2.p2, ax0);
		if (selfproj < p0.min || selfproj > p0.max) return false;

		ax0 = l2.dir().perp();
		selfproj = ax0.dot(l2.p1);
		p0 = { l1.p1, l1.p2, ax0  };
		return !(selfproj < p0.min || selfproj > p0.max);
	}

	bool intersLineQuad(Line const& l, aabb2 const& quad) {
		//cheapest check first
		vec2 ax0 = l.dir().perp();
		float selfproj = ax0.dot(l.p1);

		Projection p0(quad, ax0);
		if (selfproj < p0.min || selfproj > p0.max) return false;
		
		vec2 ax1 = v2ax::upv2;
		Projection p1(quad, ax1);
		Projection p2(l.p1, l.p2, ax1);
		if (!p1.overlaps(p2)) return false;

		vec2 ax2 = v2ax::fwv2;
		p1 = { quad, ax2 };
		p2 = { l.p1, l.p2, ax2 };
		return p1.overlaps(p2);
	}
	
	Line getPolyEdge(int i, std::vector<Vector2f> &poly, int dir) {
		return Line(poly[i], poly[util::pmod(i + dir, poly.size())]);
	}

	Line getPolyEdge(int i, std::vector<Vector2f> &poly) {
		return getPolyEdge(i, poly, 1);
	}

	int intervalsOverlap(const float &min1, const float &max1, const float &min2, const float &max2) {
		if (min1 < min2 && max1 < max2 && max1 >= min2) {
			return 0;
		}
		else if (max1 > max2 && min1 > min2 && min1 <= max2) {
			return 1;
		}
		else if (min1 < min2 && max1 > max2) {
			return 2;
		}
		else  if (min1 >= min2 && max1 <= max2) {
			return 3;
		}

		return -1;
	}

	bool intervalsOverlapCheck(const float &min1, const float &max1, const float &min2, const float &max2) {
		return intervalsOverlap(min1, max1, min2, max2) > -1;
	}

	float calcPerimeter(std::vector<Vector2f> &poly) {
		float perimeter = 0.0f;
		for (int i = 0; i < poly.size(); i++) {
			if (poly[i] != poly[(i + 1) % poly.size()]) {
				perimeter += (poly[(i + 1) % poly.size()] - poly[i]).magnitude();
			}
		}
		return perimeter;
	}

	aabb2 calcBoundingBox(std::vector<Vector2f> &poly) {
		Vector2f boundingBoxMin = Vector2f(FLT_MAX, FLT_MAX);
		Vector2f boundingBoxMax = Vector2f(-(std::numeric_limits<float>::max)(), -(std::numeric_limits<float>::max)());
		for (int i = 0; i < poly.size(); i++) {
			if (poly[i].x > boundingBoxMax.x) {
				boundingBoxMax.x = poly[i].x;
			}
			if (poly[i].y > boundingBoxMax.y) {
				boundingBoxMax.y = poly[i].y;
			}

			if (poly[i].x < boundingBoxMin.x) {
				boundingBoxMin.x = poly[i].x;
			}
			if (poly[i].y < boundingBoxMin.y) {
				boundingBoxMin.y = poly[i].y;
			}
		}

		return aabb2(boundingBoxMin, boundingBoxMax);
	}

	float calcArea(std::vector<Vector2f> &poly, int nIterations) {
		aabb2 bb = calcBoundingBox(poly);
		//Debug::drawQuad(Consts::project(pos + boundingBoxMin), (boundingBoxMax - boundingBoxMin)*Consts::zoom);

		float pointValueMult = 1.0f;
		Vector2f offset = (bb.fmax() - bb.fmin()) - (bb.fmax() - bb.fmin())*Vector2f((nIterations - 1) / (float)nIterations, (nIterations - 1) / (float)nIterations);
		int hits = 0;
		for (int y = 0; y < nIterations; y++) {
			for (int x = 0; x < nIterations; x++) {
				Vector2f point = bb.fmin() + offset / 2.0f + (bb.fmax() - bb.fmin())*Vector2f(x / (float)nIterations, y / (float)nIterations);
				if (utils2::contains(point, poly)) {
					//Debug::drawCircle(Consts::project(pos + point), 0.01f);
					hits++;
				}
				/*else {
					Debug::drawCircle(Consts::project(pos + point), 0.01f, Vector4f(0.7f,0.0f,0.0f,1.0f));
				}*/
			}
		}

		float hitPerc = hits / (float)(nIterations*nIterations);

		return (bb.fmax().x - bb.fmin().x)*(bb.fmax().y - bb.fmin().y)*hitPerc;
	}

	float calcArea(std::vector<Vector2f> &poly) {
		return calcArea(poly, 10);
	}

	std::vector<LinePolyInters> lineIntersPoly(Line l, std::vector<Vector2f> &poly, Vector2f offset) {
		std::vector <LinePolyInters> interss(0);
		for (int i = 0; i < poly.size(); i++) {
			Vector2f point;
			if (lineInters(l, Line(poly[i] + offset, poly[(i + 1) % poly.size()] + offset), point)) {
				interss.push_back(LinePolyInters(i, (point - l.p1).magnitude()));
			}
		}

		std::sort(interss.begin(), interss.end(), [](LinePolyInters const &a, LinePolyInters const &b) {
			return a.dist < b.dist;
		});

		return interss;
	}
	std::vector<LinePolyInters> lineIntersPoly(Line l, std::vector<Vector2f> &poly) { return lineIntersPoly(l, poly, Vector2f()); }

	bool lineIntersPolyCheck(Line l, std::vector<Vector2f> &poly, Vector2f offset) {
		for (int i = 0; i < poly.size(); i++) {
			Vector2f point;
			if (lineInters(l, Line(poly[i] + offset, poly[(i + 1) % poly.size()] + offset), point)) {
				return true;
			}
		}
		return false;
	}
	bool lineIntersPolyCheck(Line l, std::vector<Vector2f> &poly) { return lineIntersPolyCheck(l, poly, Vector2f()); }

	std::vector<PolyPolyInters> polyIntersPoly(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2, Vector2f offset) {
		std::vector<PolyPolyInters> polyInterss(0);

		for (int i = 0; i < poly1.size(); i++) {
			std::vector<LinePolyInters> inters = lineIntersPoly(getPolyEdge(i, poly1), poly2, offset);
			for (int i = 0; i < inters.size(); i++) {
				polyInterss.push_back(PolyPolyInters(i, inters[i].id, inters[i].dist, (getPolyEdge(i, poly1).dir()*inters[i].dist - poly2[inters[i].id]).magnitude()));
			}
		}

		return polyInterss;
	}
	std::vector<PolyPolyInters> polyIntersPoly(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2) { return polyIntersPoly(poly1, poly2, Vector2f()); }

	bool polyIntersPolyCheck(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2, Vector2f offset) {
		for (int i = 0; i < poly1.size(); i++) {
			if (lineIntersPolyCheck(getPolyEdge(i, poly1), poly2, offset)) {
				return true;
			}
		}

		return false;
	}
	bool polyIntersPolyCheck(std::vector<Vector2f> &poly1, std::vector<Vector2f> &poly2) { return polyIntersPolyCheck(poly1, poly2, Vector2f()); }


	bool contains(Vector2f point, std::vector<Vector2f> &poly, Vector2f offset) {
		int i;
		int j;
		bool result = false;
		for (i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
			Vector2f rVerti = offset + poly[i];
			Vector2f rVertj = offset + poly[j];
			if ((rVerti.y > point.y) != (rVertj.y > point.y) &&
				(point.x < (rVertj.x - rVerti.x) * (point.y - rVerti.y) / (rVertj.y - rVerti.y) + rVerti.x)) {
				result = !result;
			}
		}
		return result;
	}

	bool contains(Vector2f point, std::vector<Vector2f> &poly) {
		return contains(point, poly, Vector2f());
	}

	bool contains(std::vector<Vector2f> &container, std::vector<Vector2f> &poly, Vector2f offset) {
		for (int i = 0; i < poly.size(); i++) {
			if (!contains(poly[i], container, offset)) {
				return false;
			}
		}

		return !polyIntersPolyCheck(container, poly, offset);
	}
	bool contains(std::vector<Vector2f> &container, std::vector<Vector2f> &poly) { return contains(container, poly, 0.0f); }

	bool circleLineInters(Line l, Vector2f cPos, float radius, CircleIntersection &outInters) {
		Vector2f dist = (cPos - l.p1);
		Vector2f end = -(l.p2 - l.p1);

		float a = end.dot(end);
		float b = 2 * dist.dot(end);
		float c = dist.dot(dist) - (radius / 2.0f)*(radius / 2.0f);

		float discriminant = b * b - 4.0f * a*c;
		if (discriminant < 0.0f)
		{
			return false;
		}
		else
		{
			// ray didn't totally miss sphere,
			// so there is a solution to
			// the equation.

			discriminant = (float)sqrt(discriminant);

			// either solution may be on or off the ray so need to test both
			// t1 is always the smaller value, because BOTH discriminant and
			// a are nonnegative.
			float t1 = (-b - discriminant) / (2.0f * a);
			float t2 = (-b + discriminant) / (2.0f * a);

			// 3x HIT cases:
			//          -o->             --|-->  |            |  --|->
			// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit), 

			// 3x MISS cases:
			//       ->  o                     o ->              | -> |
			// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

			if (t1 >= 0.0f && t1 <= 1.0f)
			{
				// t1 is the intersection, and it's closer than t2
				// (since t1 uses -b - discriminant)
				// Impale, Poke
				outInters = CircleIntersection(l.p1 + ((l.p2 - l.p1)*(t1)), t1,
					l.p1 + ((l.p2 - l.p1)*(t2)), t2);
				return true;
			}

			// here t1 didn't intersect so we are either started
			// inside the sphere or completely past it
			if (t2 >= 0.0f && t2 <= 1.0f)
			{
				// ExitWound
				return false;
			}

			return false;
		}
	}

	bool circleLineInters(Line l, Vector2f cPos, float radius) {
		CircleIntersection ci;
		return circleLineInters(l, cPos, radius, ci);
	}

	bool isClockwise(std::vector<Vector2f> &points) {
		float sum = 0.0f;
		for (int i = 0; i < points.size(); i++) {
			Vector2f p1 = points[i];
			Vector2f p2 = points[(i + 1) % points.size()];
			sum += (p2.x - p1.x) * (p2.y + p1.y);
		}
		return sum > 0.0f;
	}

	std::vector<Vector2f> makeClockwise(std::vector<Vector2f> &poly) {
		if (isClockwise(poly)) return poly;

		std::vector<Vector2f> copy = poly;
		std::reverse(copy.begin(), copy.end());

		return copy;
	}

	std::vector<Vector2f> makeCounterClockwise(std::vector<Vector2f> &poly) {
		if (!isClockwise(poly)) return poly;

		std::vector<Vector2f> copy = poly;
		std::reverse(copy.begin(), copy.end());

		return copy;
	}

	Vector2f getEdge(std::vector<Vector2f> &mesh, int i) {
		return mesh[(i + 1) % mesh.size()] - mesh[i];
	}

	bool isConvex(std::vector<Vector2f> &mesh) {
		if (mesh.size() <= 2) return false;

		bool order = getEdge(mesh, 0).cross(getEdge(mesh, 1)) >= 0.0f;

		for (int i = 0; i < mesh.size(); i++) {
			bool nOrder = getEdge(mesh, i).cross(getEdge(mesh, (i + 1) % mesh.size())) >= 0.0f;

			if (nOrder != order) {
				return false;
			}
		}
		return true;
	}

	Vector2f correct(std::vector<Vector2f> &mesh, int id, int dir) {
		bool cw = isClockwise(mesh);

		int id1 = cw ? (id + 1) % mesh.size() : util::pmod(id - 1, mesh.size()),
			id2 = cw ? util::pmod(id - 1, mesh.size()) : (id + 1) % mesh.size();

		Line l1 = Line(mesh[id1], mesh[id]),
			l2 = Line(mesh[id], mesh[id2]);

		Vector2f perp1 = l1.dir().perp(),
			perp2 = l2.dir().perp();

		Vector2f perp = ((perp1 + perp2) / 2.0f).normalized();

		return mesh[id] + perp * (tolerance * (float)dir);
	}

	Vector2f correct(std::vector<Vector2f> &mesh, int id) {
		return correct(mesh, id, 1);
	}

	std::vector<Vector2f> correctTolerancePoints(std::vector<Vector2f> &poly) {
		std::vector<Vector2f> corrected = poly;

		for (int i = 0; i < corrected.size(); i++) {
			for (int j = i + 1; j < corrected.size(); j++) {
				Line l = Line(corrected[i], corrected[j]);
				float dist = l.dist();
				if (dist < tolerance) {
					if (dist == 0.0f) {
						corrected[i] = correct(corrected, i);
						corrected[j] = correct(corrected, j);
					}
					else {
						corrected[i] -= l.dir()*tolerance;
						corrected[j] += l.dir()*tolerance;
					}
				}
			}
		}

		return corrected;
	}

	bool visible(std::vector<Vector2f> &mesh, int v1, int v2) {
		//if the position distance between the two is 1 (they are directly connected) then they are always visible from each other
		if (abs(v2 - v1) <= 1 ||
			(v1 == mesh.size() - 1 && v2 == 0) ||
			(v1 == 0 && v2 == mesh.size() - 1)) return true;

		//Line l = Line(correct(mesh, v1), correct(mesh, v2));
		Line l = Line(mesh[v1], tolerance, mesh[v2], tolerance);
		if (!contains(l.p1, mesh) || !contains(l.p2, mesh)) return false;

		bool inters = false;
		for (int i = 0; i < mesh.size() && !inters; i++) {
			inters = lineInters(l, Line(mesh[i], mesh[(i + 1) % mesh.size()]));
		}

		return !inters;
	}

	std::vector<std::vector<Vector2f>> joinAdjacent(std::vector<std::vector<Vector2f>> polys) {
		if (polys.size() == 1) return polys;

		//std::cout << "polys size " << polys.size() << "\n";
		for (int i = 0; i < polys.size(); i++) {
			if (!isClockwise(polys[i])) {
				std::reverse(polys[i].begin(), polys[i].end());
			}
		}
		
		int adji = -1 , adjj = -1;
		int adjiv = -1, adjjv = -1;
		for (int i = 0; i < polys.size() && adji == -1; i++) {
			for (int i2 = i+1; i2 < polys.size() && adjj == -1; i2++) {
				
				for (int j = 0; j < polys[i].size() && adji == -1; j++) {
					for (int j2 = 0; j2 < polys[i2].size() && adji == -1; j2++) {
						if (polys[i][j].equalsRange(polys[i2][(j2 + 1) % polys[i2].size()], tolerance) &&
							polys[i][(j + 1) % polys[i].size()].equalsRange(polys[i2][j2], tolerance)) {
							
							//Debug::rtlog << "adj found!\n";

							adji = i;
							adjj = i2;

							adjiv = j;
							adjjv = j2;
						}
					}
				}
			}
		}

		if (adji == -1 || adjj == -1 || adjiv==-1 || adjjv==-1) {
			//std::cout << "no adj found!\n";
			return polys;
		}

		std::cout << adji << " " << adjj << " " << adjiv << " " << adjjv << " (" << polys[adji].size() << ", " << polys[adjj].size() << ")\n";

		std::vector<std::vector<Vector2f>> joined(0);
		joined.reserve(polys.size()-1);

		std::vector<Vector2f> nPoly(0);
		nPoly.reserve(polys[adji].size() + polys[adjj].size());

		std::cout << "inserting from 0 to " << adji << "\n";
		nPoly.insert(nPoly.end(), polys[adji].begin(), polys[adji].begin()+adjiv);
		
		std::cout << "inserting from " << ((adjjv + 1) % polys[adjj].size()) << " to " << (polys[adjj].size()-1) <<"\n";
		nPoly.insert(nPoly.end(), polys[adjj].begin()+(adjjv+1)%polys[adjj].size(), polys[adjj].end());

		std::cout << "inserting from 0 to " << adjjv << "\n";
		nPoly.insert(nPoly.end(), polys[adjj].begin(), polys[adjj].begin()+adjjv);

		std::cout << "inserting from " << ((adjiv + 1) % polys[adji].size()) <<" to " << (polys[adji].size()-1) << "\n";
		nPoly.insert(nPoly.end(), polys[adji].begin()+(adjiv+1)%polys[adji].size(), polys[adji].end());

		polys.erase(polys.begin()+adji);
		polys.erase(polys.begin() + (adjj-1));

		joined.insert(joined.end(), polys.begin(), polys.end());
		joined.push_back(nPoly);

		std::cout << "joined!\n";

		return joinAdjacent(joined);
	}

	int absDistCheck(int i, int j, int size) {
		if ((i == size-1 && j == 0) || (i == 0 && j == size-1)) return 1;
		else return abs(i - j);
	}

	SelfIntersection selfIntersects(std::vector<Vector2f> &poly, int idx, bool optimize) {
		for (int i = optimize ? idx+1 : 0; i < poly.size(); i++) {
			Vector2f point = 0.0f;
			if (i != idx && absDistCheck(i, idx, poly.size()) > 1 && lineInters(getPolyEdge(i, poly), getPolyEdge(idx, poly), point)) {
				return SelfIntersection(i, point);
			}
		}

		return SelfIntersection(-1, 0.0f);
	}
	SelfIntersection selfIntersects(std::vector<Vector2f> &poly, int idx) { return selfIntersects(poly, idx, false); }

	bool collidesWithSelf(std::vector<Vector2f> &poly) {
		for (int i = 0; i < poly.size(); i++) {
			if (selfIntersects(poly, i, true).id > -1) return true;
		}

		return false;
	}

	std::vector<std::vector<Vector2f>> decollide(std::vector<Vector2f> &poly) {
		if (poly.size() <= 0) { return {}; }

		std::vector<Vector2f> firstMesh(0);
		firstMesh.push_back(poly[0]);

		int colid = 0;
		SelfIntersection selfid(-1,0.0f);
		while ((selfid = selfIntersects(poly, colid)).id == -1) {
			colid++;
			colid %= poly.size();
			if (colid == 0) { 
				return { poly }; 
			}
		
			firstMesh.push_back(poly[colid]);
		}

		firstMesh.push_back(selfid.point);

		int endIter = (selfid.id + 1)%poly.size();
		while (endIter != 0) {
			endIter++;
			endIter %= poly.size();
			if (endIter == (selfid.id + 1) % poly.size()) { return {}; }
		
			firstMesh.push_back(poly[endIter]);
		}

		std::vector<Vector2f> secondMesh(0);

		int secondIter = (colid + 1) % poly.size();
		secondMesh.push_back(poly[secondIter]);

		while (secondIter != selfid.id) {
			secondIter++;
			secondIter %= poly.size();
			if (secondIter == (colid + 1) % poly.size()) { return {}; }
		
			secondMesh.push_back(poly[secondIter]);
		}

		secondMesh.push_back(selfid.point);

		std::vector<std::vector<Vector2f>> firstDecollided = decollide(firstMesh);
		std::vector<std::vector<Vector2f>> secondDecollided = decollide(secondMesh);

		std::vector<std::vector<Vector2f>> finalDecollided(0);
		finalDecollided.reserve(firstDecollided.size()+ secondDecollided.size());
		finalDecollided.insert(finalDecollided.end(), firstDecollided.begin(), firstDecollided.end());
		finalDecollided.insert(finalDecollided.end(), secondDecollided.begin(), secondDecollided.end());

		return finalDecollided;
	}

	int getDist(int i, int j, int size, int dir) {
		if (i <= j && dir >= 1) return j - i;
		else if (i > j && dir >= 1) return (size - i) + j;
		else if (i < j && dir <= -1) return i + (size - j);
		else if (i >= j && dir <= -1) return i-j;
		else return 0;
	}

	void removeFrom(std::vector<Vector2f> &mesh, int from, int to, int dir) {
		if (from <= to && dir >= 1) mesh.erase(mesh.begin() + from, mesh.begin() + to);
		else if (from > to && dir >= 1) {
			mesh.erase(mesh.begin() + from, mesh.end());
			mesh.erase(mesh.begin(), mesh.begin() + to);
		}
		else if (from <= to && dir <= -1) {
			mesh.erase(mesh.begin(), mesh.begin() + from);
			mesh.erase(mesh.begin() + to, mesh.end()-from);
		}
		else if (from > to && dir <= -1) mesh.erase(mesh.begin()+to, mesh.begin()+from); 
	}

	std::vector<std::vector<Vector2f>> makeConvexOld(std::vector<Vector2f> mesh, int startMeshSize, int level) {
		//Debug::log << "rec level convex " << level << " " << startMeshSize << "\n";
		if (level > startMeshSize) {
			return {  mesh }; 
		}
		if (mesh.size() < 3) { /*VLDEBUG("error: mesh has < 3 vertices\n");*/ return { }; }
		if (mesh.size() == 3 || isConvex(mesh)) return { mesh };

		mesh = makeClockwise(mesh);

		int reflexVert = -1;

		for (int i = 0; i < mesh.size() && reflexVert == -1; i++) {
			if (getPolyEdge(i, mesh).dir().cross(getPolyEdge((i + 1) % mesh.size(), mesh).dir()) > 0.0f) {
				reflexVert = (i + 1) % mesh.size();
			}
		}
		if (reflexVert == -1) { return { mesh }; }
		//VDEBUG("found reflexVert at " << reflexVert << "\n");

		std::vector<Vector2f> firstMesh(0);
		firstMesh.push_back(mesh[reflexVert]);
		firstMesh.push_back(mesh[(reflexVert + 1) % mesh.size()]);

		int iterVert = (reflexVert + 2) % mesh.size();
		firstMesh.push_back(mesh[iterVert]);

		while (!visible(mesh, iterVert, reflexVert)) {
			//VDEBUG("vert " << iterVert << " is not visible from reflexVert\n");
			iterVert++;
			iterVert %= mesh.size();

			firstMesh.push_back(mesh[iterVert]);

			if (iterVert == (reflexVert + 2) % mesh.size()) { return {  mesh }; }
		}

		std::vector<Vector2f> secondMesh(0);

		int secondIter = iterVert;
		secondMesh.push_back(mesh[secondIter]);

		while (secondIter != reflexVert) {
			secondIter++;
			secondIter %= mesh.size();

			secondMesh.push_back(mesh[secondIter]);

			if (secondIter == iterVert) { return { }; }
		}

		std::vector<std::vector<Vector2f>> firstConvex = makeConvexOld(firstMesh, startMeshSize, level + 1);
		
		std::vector<std::vector<Vector2f>> secondConvex = makeConvexOld(secondMesh, startMeshSize, level + 1);
		

		std::vector<std::vector<Vector2f>> finalMesh(0);
		/*for (int i = 0; i < firstConvex.size(); i++) {
			finalMesh.push_back(firstConvex[i]);
		}
		for (int i = 0; i < secondConvex.size(); i++) {
			finalMesh.push_back(secondConvex[i]);
		}*/
		finalMesh.reserve(firstConvex.size() + secondConvex.size());
		finalMesh.insert(finalMesh.end(), firstConvex.begin(), firstConvex.end());
		finalMesh.insert(finalMesh.end(), secondConvex.begin(), secondConvex.end());

		return finalMesh;
	}
	
	std::vector<std::vector<Vector2f>> makeConvexI(std::vector<Vector2f> &mesh) {
		if (mesh.size() == 3 || isConvex(mesh))  return { mesh };
		if (mesh.size() < 3) return {};

		std::vector<std::vector<Vector2f>> nonConvex = { makeClockwise(mesh) };
		std::vector<std::vector<Vector2f>> convex(0);

		while (nonConvex.size() > 0) {
			
			std::vector<Vector2f> &cur = nonConvex[nonConvex.size()-1];

			int reflexVert = -1;

			/*for (int i = 0; i < cur.size() && reflexVert == -1; i++) {
				if (getPolyEdge(i, cur).dir().cross(getPolyEdge((i + 1) % cur.size(), cur).dir()) > 0.0f) {
					reflexVert = (i + 1) % cur.size();
				}
			}*/

			int i = cur.size() / 2;
			bool cycle = false;
			while (!cycle || i != cur.size() / 2) {

				if (getPolyEdge(i, cur).dir().cross(getPolyEdge((i + 1) % cur.size(), cur).dir()) > 0.0f) {
					reflexVert = (i + 1) % cur.size();
					break;
				}


				cycle = true;
				i++;
				i %= cur.size();
			}
			if (reflexVert == -1) { 
				convex.push_back(cur);
				nonConvex.pop_back(); 
				continue; 
			}

			//std::vector<int> toRemove(0);
			std::vector<Vector2f> otMesh(0);
			otMesh.push_back(cur[reflexVert]);
			otMesh.push_back(cur[(reflexVert+1)%cur.size()]);

			//toRemove.push_back((reflexVert + 1) % cur.size());

			int iterVert = (reflexVert + 2) % cur.size();
			int toRemove = iterVert;
			otMesh.push_back(cur[iterVert]);

			 cycle = false;
			while (!visible(cur, iterVert, reflexVert)) {
				//toRemove = iterVert;
				//toRemove.push_back(iterVert);
				iterVert++;
				iterVert %= cur.size();
				toRemove = iterVert;
				otMesh.push_back(cur[iterVert]);

				//toRemove = iterVert;
				cycle = true;
				if (iterVert == (reflexVert + 2) % cur.size()) { nonConvex.pop_back(); convex.push_back(cur); break; }
			}
			if (cycle && iterVert == (reflexVert + 2) % cur.size()) {  nonConvex.pop_back(); convex.push_back(cur); continue; }
			//if (cur.size() > 3 && getDist(iterVert, reflexVert, cur.size(), 1) <= 1) { VLDEBUG("error: dist from iterVert to reflexVert <= 1\n"); nonConvex.pop_back(); convex.push_back(cur); continue;}
			
			if (cur.size() == otMesh.size()) {
				//VLDEBUG("error: cur size and otMesh size should not be equal\n");
				bool equal = true;
				for (int i = 0; i < cur.size() && equal; i++) {
					equal = cur[i] == otMesh[i];
				}
				if (equal) {
					convex.push_back(cur);
					nonConvex.pop_back();
					continue;
				}
				else {
					bool allEqual = true;
					for (int i = 0; i < cur.size() && allEqual; i++) {
						bool equals = false;
						for (int j = 0; j < otMesh.size() && !equals; j++) {
							equals = cur[i] == otMesh[j];
						}
						allEqual = equals;
					}

					convex.push_back(cur);
					nonConvex.pop_back();
					continue;
				}
			}

			/*for (int i = 0; i < toRemove.size(); i++) {
				cur.erase(cur.begin()+ toRemove[i]-i);
			}*/
			removeFrom(cur, (reflexVert + 1) % cur.size(), toRemove, 1);

			int oldMPos = nonConvex.size() - 1;
			if (isConvex(otMesh)) { convex.push_back(otMesh); }
			else { nonConvex.push_back(otMesh); }

			
			if (nonConvex[oldMPos].size() < 3) { nonConvex.erase(nonConvex.begin()+ oldMPos); }
		}

		return convex;
	}
	
	std::vector<std::vector<Vector2f>> makeConvex(std::vector<Vector2f> mesh) { 
#ifdef ENABLE_DECOLLISION
		if (collidesWithSelf(mesh)) {
			std::vector<std::vector<Vector2f>> decollided = decollide(mesh);
			std::vector<std::vector<Vector2f>> deConvexed(0);
			for (int i = 0; i < decollided.size(); i++) {
				std::vector<std::vector<Vector2f>> convexed = makeConvex(decollided[i], mesh.size(), 0);

				deConvexed.reserve(deConvexed.size() + convexed.size());
				deConvexed.insert(deConvexed.end(), convexed.begin(), convexed.end());
			}
			return deConvexed;
		}
		else {
			return makeConvex(mesh, mesh.size(), 0);
		}
#endif
#ifndef  ENABLE_DECOLLISION
		//return makeConvexOld(mesh, mesh.size(), 0); 
		return makeConvexI(mesh);
#endif
	}

	std::vector<BrokenPoly> slicePoly(Line l, std::vector<Vector2f> &poly) {
		if (contains(l.p1, poly)) return {};

		std::vector<LinePolyInters> interss = lineIntersPoly(l, poly);
		if (interss.size() < 2 || interss[0].id == interss[1].id) return {};

		std::vector<BrokenPoly> polys(0);

		std::vector<Vector2f> firstVerts(0);

		int iter = interss[0].id;
		firstVerts.push_back(l.p1 + l.dir()*interss[0].dist);
		while (iter != interss[1].id) {
			iter++;
			iter %= poly.size();
			firstVerts.push_back(poly[iter]);
		}
		firstVerts.push_back(l.p1 + l.dir()*interss[1].dist);

		Vector2f nPos;
		for (int i = 0; i < firstVerts.size(); i++) {
			nPos += firstVerts[i];
		}
		nPos /= (float)firstVerts.size();

		for (int i = 0; i < firstVerts.size(); i++) {
			firstVerts[i] -= nPos;
		}

		polys.push_back(BrokenPoly(nPos, firstVerts));

		std::vector<Vector2f> secondVerts(0);

		iter = interss[0].id;
		secondVerts.push_back(l.p1 + l.dir()*interss[0].dist);
		secondVerts.push_back(poly[iter]);
		while (iter != (interss[1].id + 1) % poly.size()) {
			iter--;
			iter = util::pmod(iter, poly.size());
			secondVerts.push_back(poly[iter]);
		}
		secondVerts.push_back(l.p1 + l.dir()*interss[1].dist);

		nPos = Vector2f();
		for (int i = 0; i < secondVerts.size(); i++) {
			nPos += secondVerts[i];
		}
		nPos /= (float)secondVerts.size();

		for (int i = 0; i < secondVerts.size(); i++) {
			secondVerts[i] -= nPos;
		}

		polys.push_back(BrokenPoly(nPos, secondVerts));

		return polys;
	}

	std::vector<BrokenPoly> breakPoly(Vector2f center, std::vector<Vector2f> &poly) {
		if (!contains(center, poly)) {
			return {  };
		}

		std::vector<BrokenPoly> polys(0);

		for (int i = 0; i < poly.size(); i++) {
			Line l(center, 0.0f, poly[i] + (poly[(i + 1) % poly.size()] - poly[i])*0.5f, tolerance*2.0f);
			if (lineIntersPolyCheck(l, poly)) continue;

			std::vector<Vector2f> nVerts = std::vector<Vector2f>(0);

			nVerts.push_back(center);
			nVerts.push_back(poly[i] + (poly[(i + 1) % poly.size()] - poly[i])*0.5f);
			int j = (i + 1) % poly.size();

			while (lineIntersPolyCheck(Line(center, 0.0f, poly[j] + (poly[(j + 1) % poly.size()] - poly[j])*0.5f, tolerance*2.0f), poly)) {
				nVerts.push_back(poly[j]);
				j++;
				j %= poly.size();
				if (j == i) { return {  }; }
			}
			nVerts.push_back(poly[j]);
			nVerts.push_back(poly[j] + (poly[(j + 1) % poly.size()] - poly[j])*0.5f);

			Vector2f nPos = Vector2f();
			for (int k = 0; k < nVerts.size(); k++) {
				nPos += nVerts[k];
			}
			nPos /= (float)nVerts.size();

			for (int k = 0; k < nVerts.size(); k++) {
				nVerts[k] -= nPos;
			}
			polys.push_back(BrokenPoly(nPos, nVerts));
		}

		return polys;
	}

	BrokenPoly getBrokenPoly(std::vector<Vector2f> poly, float perimeter, float area) {
		Vector2f nPos;
		for (int i = 0; i < poly.size(); i++) {
			nPos += poly[i];
		}
		nPos /= (float)poly.size();

		for (int i = 0; i < poly.size(); i++) {
			poly[i] -= nPos;
		}
		return BrokenPoly(nPos,poly, perimeter, area);
	}

#ifdef BDEBUGEN
#define BDEBUG(str) do { Debug::log << str; } while( false )
#else
#define BDEBUG(str) do { } while ( false )
#endif
#ifdef BLDEBUGEN
#define BLDEBUG(str) do { Debug::log << str; } while( false )
#else
#define BLDEBUG(str) do { } while ( false )
#endif
	/*
		-1: find the first non-contained (in the destructor) vertex (of the poly)
		-2: add the vertices until you find an intersection with the destructor
		-3: 'jump' on the destructor and go in the right direction (meaning staying inside the poly), 
			adding the vertices as you go, until you find an intersection with the poly
		-4: 'jump' again on the poly and add all the remaining vertices until you find the first one
			you already added
		-5: create the next mesh from the 'other side' of this intersection
		-6: apply the algorithm again on the 2 newly generated meshes
	*/
	std::vector<BrokenPoly> breakPoly(std::vector<Vector2f> destructor, std::vector<Vector2f> poly, int startMeshSize, int level) {
		//Debug::log << "rec level " << level << " (" << destructor.size() << " " << poly.size() << ")\n";
		BDEBUG("recursion level " << level << "\n");
#ifdef BDEBUGEN
		BDEBUG("poly (" << poly.size() << "):\n");
		for (int i = 0; i < poly.size(); i++) {
			BDEBUG(i << ": " << poly[i].toString() << "\n");
		}
		BDEBUG("destructor (" << destructor.size() << "):\n");
		for (int i = 0; i < destructor.size(); i++) {
			BDEBUG(i << ": " << destructor[i].toString() << "\n");
		}
#endif
		
		if (destructor.size() <= 2 || poly.size() <= 2) { BLDEBUG("error: destructor or poly size <= 2\n"); return {}; }
		
		/*for (int i = 0; i < poly.size(); i++) {
			if (poly[i] == poly[(i + 1) % poly.size()] || isnan(poly[i].x) ||  isnan(poly[i].y)) poly.erase(poly.begin() + i);
			for (int j = i+1; j < poly.size(); j++) {
				if (poly[i] == poly[j]) {
#ifdef BLDEBUGEN
					BLDEBUG("poly (" << poly.size() << "):\n");
					for (int i = 0; i < poly.size(); i++) {
						BLDEBUG(i << ": " << poly[i].toString() << "\n");
					}
					BLDEBUG("error: 2 vertices of poly are equal ("<< level << " " << poly.size() <<")\n");
#endif
					return { getBrokenPoly(poly, calcPerimeter(poly), calcArea(poly,5)) };
				}
			}
		}*/

		if (level > startMeshSize) { 
#ifdef BLDEBUGEN
			BLDEBUG("poly (" << poly.size() << "):\n");
			for (int i = 0; i < poly.size(); i++) {
				BLDEBUG(i << ": " << poly[i].toString() << "\n");
			}
			BLDEBUG("error: recursion level > startMeshSize (" << level << " " << startMeshSize << ")\n");
#endif
			return { getBrokenPoly(poly, calcPerimeter(poly), calcArea(poly, 5)) }; 
		}

		float perimeter = calcPerimeter(poly);
		if (perimeter <= tolerance) { BLDEBUG("error: poly perimeter <= tolerance (" << perimeter << ")\n"); return {}; }
		float area = calcArea(poly, 5);
		BDEBUG("area: " << area << "\n");
		if (area <= areaTolerance) { BLDEBUG("error: poly area <= tolerance (" << area <<  ")\n"); return {}; }

		/*for (int i = 0; i < poly.size(); i++) {
			if (getPolyEdge(i, poly).dist() <= tolerance) {
				Vector2f mean = poly[i] + poly[(i + 1) % poly.size()] / 2.0f;
				if (i == poly.size() - 1) {
					poly.erase(poly.begin() + i);
					poly.erase(poly.begin());
					poly.insert(poly.begin(), mean);
				}
				else {
					poly.erase(poly.begin() + i, poly.begin() + i+1);
					poly.insert(poly.begin() + i, mean);
				}
				BDEBUG("verts where too near, removed\n");
			}
		}*/

		destructor = makeCounterClockwise(destructor);
		poly = makeClockwise(poly);

		//-1:---------------------------------------

		bool allContained = false;

		int first_vert = 0;
		while (contains(poly[first_vert], destructor)) {
			first_vert++;
			first_vert %= poly.size();
			if (first_vert == 0) { 
				//std::cout << "error: first finding loop wrapped around\n"; return {};

				BDEBUG("all vertices are contained\n");
				//return { BrokenPoly(0.0f, poly) };
				allContained = true;
				break;
			}
		}

		if (allContained) {
			destructor = makeClockwise(destructor);
			
#ifdef BDEBUGEN
			BDEBUG("destructor (" << destructor.size() << "):\n");
			for (int i = 0; i < destructor.size(); i++) {
				BDEBUG(i << ": " << destructor[i].toString() << "\n");
			}
#endif
		}

		BDEBUG("first_vert " << first_vert << "\n");

		//-2:---------------------------------------

		std::vector<Vector2f> firstMesh(0);

		int polyInIter = first_vert;
		firstMesh.push_back(poly[polyInIter]);
		while (!lineIntersPolyCheck(getPolyEdge(polyInIter, poly), destructor)) {
			polyInIter++;
			polyInIter %= poly.size();
			firstMesh.push_back(poly[polyInIter]);
			if (polyInIter == first_vert) {
				BDEBUG("poly has no intersection with destructor, returning\n");

				return { getBrokenPoly(poly, perimeter, area) };
			}
		}

		Line polyInEdge = getPolyEdge(polyInIter, poly);
		std::vector<LinePolyInters> inInters = lineIntersPoly(polyInEdge, destructor);
		if (inInters.size() == 0) {
			BLDEBUG("error: inInters size should be at least 1\n");
			return { getBrokenPoly(poly, perimeter, area) };
		}
		if (inInters[0].dist <= tolerance) {
			BLDEBUG("error: inInters[0].dist <= tolerance\n");
			return { getBrokenPoly(poly, perimeter, area) };
		}

		BDEBUG("intersection with destructor found at " << polyInIter << " (id: " << inInters[0].id << ", dist: " << inInters[0].dist << ")\n");
		
		//-3:---------------------------------------
		
		std::vector<Vector2f> secondMesh(0);

		Vector2f intersPoint = polyInEdge.p1+ polyInEdge.dir()*inInters[0].dist;
		firstMesh.push_back(intersPoint - getPolyEdge(polyInIter, poly).dir()*tolerance);
		secondMesh.push_back(intersPoint + getPolyEdge(polyInIter, poly).dir()*tolerance);

		bool looped = false;
		int destIter = inInters[0].id;
		BDEBUG("destIter " << destIter << "\n");
		Vector2f checkIntersPoint = intersPoint+getPolyEdge(inInters[0].id, destructor).dir()*tolerance;
		while (!lineIntersPolyCheck(Line(checkIntersPoint, destructor[(destIter+1)%destructor.size()]), poly)) {
			BDEBUG("checkIntersPoint doesn't intersect with " << ((destIter + 1) % destructor.size()) << "\n");
			destIter++;
			destIter %= destructor.size();
			if (looped && destIter == (inInters[0].id+1)%destructor.size()) { BLDEBUG("error: destIter loop wrapped around\n"); return {}; }
			
			looped = true;

			firstMesh.push_back(correct(destructor, destIter, !allContained?-1:1));
			secondMesh.push_back(correct(destructor, destIter, !allContained ? 1 : -1));

			checkIntersPoint = destructor[destIter];
		}
		
		Line destEdge = Line(checkIntersPoint, destructor[(destIter + 1) % destructor.size()]);
		std::vector<LinePolyInters> outInters = lineIntersPoly(destEdge, poly);
		if (outInters.size() == 0) {
			BLDEBUG("error: outInters size should be at least 1\n");
			return { getBrokenPoly(poly, perimeter, area) };
		}
		if (outInters[0].dist <= tolerance) {
			BLDEBUG("error: outInters[0].dist <= tolerance\n");
			return { getBrokenPoly(poly, perimeter, area) };
		}

		BDEBUG("checkIntersPoint intersects with poly (destIter+1 " << ((destIter + 1) % destructor.size()) << ", poly id " << outInters[0].id << ")\n");

		Vector2f outIntersPoint = checkIntersPoint+destEdge.dir()*outInters[0].dist;

		Vector2f polyOutCorrection = getPolyEdge(outInters[0].id, poly).dir()*tolerance;
		firstMesh.push_back(outIntersPoint + polyOutCorrection);
		secondMesh.push_back(outIntersPoint - polyOutCorrection);

		int firstAssembleIter = (outInters[0].id + 1) % poly.size();
		while (firstAssembleIter != first_vert) {
			firstMesh.push_back(poly[firstAssembleIter]);

			firstAssembleIter++;
			firstAssembleIter %= poly.size();
			if (firstAssembleIter == (outInters[0].id + 1) % poly.size()) { BLDEBUG("error: firstAssembleIter loop wrapped around\n"); return {}; }
		}

		if (outInters[0].id != polyInIter) {
			int secondAssembleIter = outInters[0].id;
			secondMesh.push_back(poly[secondAssembleIter]);
			while (secondAssembleIter != (polyInIter+1)%poly.size()) {
				secondAssembleIter--;
				secondAssembleIter = util::pmod(secondAssembleIter, poly.size());
				if (secondAssembleIter == outInters[0].id) { BLDEBUG("error: secondAssembleIter loop wrapped around"); return {}; }
				
				secondMesh.push_back(poly[secondAssembleIter]);
			}
		}

		/*if (firstMesh.size() == poly.size()) {
			bool equal = true;
			for (int i = 0; i < firstMesh.size() && equal; i++)
				equal = firstMesh[i] == poly[i];

			if (equal) { 
				BDEBUG("error: firstMesh == poly"); return { getBrokenPoly(poly, perimeter, area) }; 
			}

			equal = true;
			for (int i = 0; i < poly.size() && equal; i++) {
				bool equalsVert = false;
				for (int j = i + 1; j < firstMesh.size() && !equalsVert; j++) {
					equalsVert = firstMesh[j] == poly[i];
				}
				equal = equalsVert;
			}

			if (equal) { 
				BDEBUG("error: firstMesh == poly (not in order)"); return { getBrokenPoly(poly, perimeter, area) }; 
			}
		}

		if (secondMesh.size() == poly.size()) {
			bool equal = true;
			for (int i = 0; i < secondMesh.size() && equal; i++)
				equal = secondMesh[i] == poly[i];

			if (equal) { 
				BDEBUG("error: secondMesh == poly"); return { getBrokenPoly(poly, perimeter, area) }; 
		}
		
			equal = true;
			for (int i = 0; i < poly.size() && equal; i++) {
				bool equalsVert = false;
				for (int j = i + 1; j < secondMesh.size() && !equalsVert; j++) {
					equalsVert = secondMesh[j] == poly[i];
				}
				equal = equalsVert;
			}

			if (equal) { 
				BDEBUG("error: secondMesh == poly (not in order)"); return { getBrokenPoly(poly, perimeter, area) };
			}
		}

		if (firstMesh.size() == secondMesh.size()) {
			bool equal = true;
			for (int i = 0; i < secondMesh.size() && equal; i++)
				equal = secondMesh[i] == firstMesh[i];

			if (equal) { 
				BDEBUG("error: secondMesh == firstMesh"); return { getBrokenPoly(firstMesh, perimeter, area) }; 
			}

			equal = true;
			for (int i = 0; i < firstMesh.size() && equal; i++) {
				bool equalsVert = false;
				for (int j = i + 1; j < secondMesh.size() && !equalsVert; j++) {
					equalsVert = secondMesh[j] == firstMesh[i];
				}
				equal = equalsVert;
			}

			if (equal) { 
				BDEBUG("error: secondMesh == firstMesh (not in order)"); return { getBrokenPoly(firstMesh, perimeter, area) }; 
			}
		}*/

		BDEBUG("first mesh " << firstMesh.size() << " second mesh " << secondMesh.size() << "\n");

		//return { BrokenPoly(0.0f, firstMesh), BrokenPoly(0.0f, secondMesh) };

		std::vector<BrokenPoly> firstBroken = breakPoly(destructor, firstMesh, startMeshSize, level+1);
		BDEBUG("firstMesh done\n");
		std::vector<BrokenPoly> secondBroken = breakPoly(destructor, secondMesh, startMeshSize, level + 1);
		BDEBUG("secondMesh done\n");

		std::vector<BrokenPoly> finalMesh(0);

		finalMesh.reserve(firstBroken.size() + secondBroken.size());
		finalMesh.insert(finalMesh.end(), firstBroken.begin(), firstBroken.end());
		finalMesh.insert(finalMesh.end(), secondBroken.begin(), secondBroken.end());

		return finalMesh;
	}

	std::vector<BrokenPoly> breakPoly(std::vector<Vector2f> destructor, std::vector<Vector2f> poly) {
#ifdef ENABLE_DECOLLISION
		if (collidesWithSelf(poly)) {
			std::vector<std::vector<Vector2f>> decollided = decollide(poly);
			std::vector<BrokenPoly> deConvexed(0);
			for (int i = 0; i < decollided.size(); i++) {
				std::vector<BrokenPoly> convexed = breakPoly(destructor, decollided[i], poly.size(), 0);

				deConvexed.reserve(deConvexed.size() + convexed.size());
				deConvexed.insert(deConvexed.end(), convexed.begin(), convexed.end());
			}
			return deConvexed;
		}
		else {
			return breakPoly(destructor, poly, poly.size(), 0);
		}
#endif
#ifndef  ENABLE_DECOLLISION
		return breakPoly(destructor, poly, poly.size(), 0);
#endif
	}
}