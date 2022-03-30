#pragma once

#include <sstream>
#include <array>
#include <vector>

#include "../Vector2f.h"

namespace prb {
	class AxisAlignedBoundingBox2D {
	protected:
		vec2 vmin = 0.f, vmax = 0.f;

		void align();
	public:

		bool doalign = true;

		AxisAlignedBoundingBox2D();
		AxisAlignedBoundingBox2D(float v);	//causes ambiguity with vec2 operators
		AxisAlignedBoundingBox2D(vec2 v);
		AxisAlignedBoundingBox2D(vec2 min, vec2 max);
		AxisAlignedBoundingBox2D(vec2 min, vec2 max, bool doalign);
		AxisAlignedBoundingBox2D(std::vector<vec2> const& shape);

		void fmaxr(vec2 max);
		vec2 fmax() const;

		void fminr(vec2 min);
		vec2 fmin() const;

		vec2 center() const;						//centers this bounding box around the origin (0,0) maintaining its original size
		AxisAlignedBoundingBox2D centered() const;	//returns a new bounding box that is centered around the origin (0,0) maintaining its original size

		float minx() const;
		float miny() const;

		float maxx() const;
		float maxy() const;

		void maxxr(float f);	//modifies max.x of the object itself
		void maxyr(float f);	//modifies max.y of the object itself

		void minxr(float f);	//modifies min.x of the object itself
		void minyr(float f);	//modifies min.y of the object itself

		AxisAlignedBoundingBox2D maxx(float f) const;	//returns a new aabb2 with max.x = f
		AxisAlignedBoundingBox2D maxy(float f) const;	//returns a new aabb2 with max.y = f
		AxisAlignedBoundingBox2D minx(float f) const;	//returns a new aabb2 with min.x = f
		AxisAlignedBoundingBox2D miny(float f) const;	//returns a new aabb2 with min.y = f
		AxisAlignedBoundingBox2D fmax(vec2 vmax) const;	//returns a new aabb2 with max = vmax;
		AxisAlignedBoundingBox2D fmin(vec2 vmin) const;	//returns a new aabb2 with min = vmin;

		vec2 get(int i) const;						//returns the n-th vertex as in:  0: vmin, 1: { vmin.x, vmax.y }, 2: vmax, 3: { vmax.x, vmin.y }
		float getSingle(int i) const;				//returns:	 0: min.x, 1: min.y, 2: max.x, 3: max.y
		vec2 operator[](int const& i) const;		//returns the n-th vertex as in:  0: vmin, 1: { vmin.x, vmax.y }, 2: vmax, 3: { vmax.x, vmin.y }
		std::array<vec2, 4> toVerts() const;		//returns the bounding box as a normal quad
		std::vector<vec2> toVec() const;			//returns the bounding box as a vector of vec2s

		vec2 edgeCenter(int i) const;	//returns the point between bb[i] and bb[(i+1)%4]
		vec2 top() const;				//alias for edgeCenter(0);
		vec2 right() const;				//alias for edgeCenter(1);
		vec2 left() const;				//alias for edgeCenter(2);
		vec2 bottom() const;			//alias for edgeCenter(3);

		bool intersects(AxisAlignedBoundingBox2D const& bb) const;

		void rescale(vec2 const& v);							//rescales the bounding box to fit v inside of it
		AxisAlignedBoundingBox2D rescaled(vec2 const& v) const; //rescales the bounding box to fit v inside of it

		void narrow(AxisAlignedBoundingBox2D const& bb);
		AxisAlignedBoundingBox2D narrowed(AxisAlignedBoundingBox2D const& bb) const;

		//rescales and translates the bounding box to fit inside another bounding box maintaining its aspect ratio
		void fit(AxisAlignedBoundingBox2D const& bb);
		AxisAlignedBoundingBox2D fitted(AxisAlignedBoundingBox2D const& bb);

		void scale(vec2 const& v);
		AxisAlignedBoundingBox2D scaled(vec2 const& scale) const; //returns a new bounding box that is scaled around the center of the original

		AxisAlignedBoundingBox2D movedIn(vec2 const& delta) const;	//returns a new bounding box that is narrowed towards its center by 'delta' units
		AxisAlignedBoundingBox2D movedOut(vec2 const& delta) const; //returns a new bounding box that is extended outwards from its center by 'delta' units

		AxisAlignedBoundingBox2D forcedIn(AxisAlignedBoundingBox2D const& bb) const; //translates this bounding box inside 'bb', changing its size if necessary

		AxisAlignedBoundingBox2D operator+(vec2 const& v) const;
		AxisAlignedBoundingBox2D operator-(vec2 const& v) const;
		AxisAlignedBoundingBox2D operator*(vec2 const& v) const;
		AxisAlignedBoundingBox2D operator/(vec2 const& v) const;

		AxisAlignedBoundingBox2D operator+(AxisAlignedBoundingBox2D const& bb) const;
		AxisAlignedBoundingBox2D operator-(AxisAlignedBoundingBox2D const& bb) const;
		AxisAlignedBoundingBox2D operator*(AxisAlignedBoundingBox2D const& bb) const;
		AxisAlignedBoundingBox2D operator/(AxisAlignedBoundingBox2D const& bb) const;

		AxisAlignedBoundingBox2D operator+=(vec2 const& v);
		AxisAlignedBoundingBox2D operator-=(vec2 const& v);
		AxisAlignedBoundingBox2D operator*=(vec2 const& v);
		AxisAlignedBoundingBox2D operator/=(vec2 const& v);

		AxisAlignedBoundingBox2D operator+=(AxisAlignedBoundingBox2D const& bb);
		AxisAlignedBoundingBox2D operator-=(AxisAlignedBoundingBox2D const& bb);
		AxisAlignedBoundingBox2D operator*=(AxisAlignedBoundingBox2D const& bb);
		AxisAlignedBoundingBox2D operator/=(AxisAlignedBoundingBox2D const& bb);

		AxisAlignedBoundingBox2D operator+(float const& v) const;
		AxisAlignedBoundingBox2D operator-(float const& v) const;
		AxisAlignedBoundingBox2D operator*(float const& v) const;
		AxisAlignedBoundingBox2D operator/(float const& v) const;

		AxisAlignedBoundingBox2D operator+=(float const& v);
		AxisAlignedBoundingBox2D operator-=(float const& v);
		AxisAlignedBoundingBox2D operator*=(float const& v);
		AxisAlignedBoundingBox2D operator/=(float const& v);

		bool operator==(float f) const;
		bool operator!=(float f) const;

		bool operator==(AxisAlignedBoundingBox2D const& f) const;
		bool operator!=(AxisAlignedBoundingBox2D const& f) const;

		bool inside(AxisAlignedBoundingBox2D const& bb) const;

		bool pointInside(vec2 const& v) const;

		vec2 size() const;
		float sizex() const;
		float sizey() const;

		std::string toString() const;
	};

	AxisAlignedBoundingBox2D operator+ (vec2 const& v, AxisAlignedBoundingBox2D const& bb);
	AxisAlignedBoundingBox2D operator- (vec2 const& v, AxisAlignedBoundingBox2D const& bb);
	AxisAlignedBoundingBox2D operator* (vec2 const& v, AxisAlignedBoundingBox2D const& bb);
	AxisAlignedBoundingBox2D operator/ (vec2 const& v, AxisAlignedBoundingBox2D const& bb);

	std::ostream& operator<<(std::ostream& os, AxisAlignedBoundingBox2D const& bb);
	std::wostream& operator<<(std::wostream& os, AxisAlignedBoundingBox2D const& bb);

	AxisAlignedBoundingBox2D aabb2s(vec2 const& size); //returns a bounding box equal to { -size/2.f, size/2.f } 

	typedef AxisAlignedBoundingBox2D aabb2;
}
