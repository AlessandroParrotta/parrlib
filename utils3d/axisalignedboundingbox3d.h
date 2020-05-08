#pragma once

#include "../vector3f.h"

class AxisAlignedBoundingBox3D{
protected:
	vec3 vmin, vmax;

	void align();
public:

	bool doalign = true;

	AxisAlignedBoundingBox3D();
	AxisAlignedBoundingBox3D(vec3 vmin, vec3 vmax);

	float minx() const;
	float miny() const;
	float minz() const;

	float maxx() const;
	float maxy() const;
	float maxz() const;

	AxisAlignedBoundingBox3D maxx(float f) const;	//returns a new aabb3 with max.x = f
	AxisAlignedBoundingBox3D maxy(float f) const;	//returns a new aabb3 with max.y = f
	AxisAlignedBoundingBox3D maxz(float f) const;	//returns a new aabb3 with max.z = f

	AxisAlignedBoundingBox3D minx(float f) const;	//returns a new aabb3 with min.x = f
	AxisAlignedBoundingBox3D miny(float f) const;	//returns a new aabb3 with min.y = f
	AxisAlignedBoundingBox3D minz(float f) const;	//returns a new aabb3 with min.z = f

	AxisAlignedBoundingBox3D fmax(vec3 vmax) const;	//returns a new aabb3 with max = vmax;
	AxisAlignedBoundingBox3D fmin(vec3 vmin) const;	//returns a new aabb3 with min = vmin;

	vec3 get(int i) const;
	vec3 operator[](int i) const;

	vec3 edgeCenter(int i) const;	//returns the point between bb[i] and bb[(i+1)%8]

	bool intersects(AxisAlignedBoundingBox3D const& bb) const;

	AxisAlignedBoundingBox3D scaled(vec3 const& scale) const;

	AxisAlignedBoundingBox3D operator+(vec3 const& v) const;
	AxisAlignedBoundingBox3D operator-(vec3 const& v) const;
	AxisAlignedBoundingBox3D operator*(vec3 const& v) const;
	AxisAlignedBoundingBox3D operator/(vec3 const& v) const;

	AxisAlignedBoundingBox3D operator+(AxisAlignedBoundingBox3D const& bb) const;
	AxisAlignedBoundingBox3D operator-(AxisAlignedBoundingBox3D const& bb) const;
	AxisAlignedBoundingBox3D operator*(AxisAlignedBoundingBox3D const& bb) const;
	AxisAlignedBoundingBox3D operator/(AxisAlignedBoundingBox3D const& bb) const;

	AxisAlignedBoundingBox3D& operator+=(vec3 const& v);
	AxisAlignedBoundingBox3D& operator-=(vec3 const& v);
	AxisAlignedBoundingBox3D& operator*=(vec3 const& v);
	AxisAlignedBoundingBox3D& operator/=(vec3 const& v);

	AxisAlignedBoundingBox3D& operator+=(AxisAlignedBoundingBox3D const& bb);
	AxisAlignedBoundingBox3D& operator-=(AxisAlignedBoundingBox3D const& bb);
	AxisAlignedBoundingBox3D& operator*=(AxisAlignedBoundingBox3D const& bb);
	AxisAlignedBoundingBox3D& operator/=(AxisAlignedBoundingBox3D const& bb);

	vec3 fmin() const;
	vec3 fmax() const;

	vec3 center() const;
	AxisAlignedBoundingBox3D centered() const;

	bool inside(AxisAlignedBoundingBox3D const& bb) const;

	bool pointInside(vec3 const& v) const;
};

typedef AxisAlignedBoundingBox3D aabb3;
