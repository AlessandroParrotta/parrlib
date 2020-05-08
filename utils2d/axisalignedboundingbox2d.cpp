#include "AxisAlignedBoundingBox2D.h"

#include <iostream>

#include "../Matrix3f.h"
#include "../otherutil.h"
#include "../stringutils.h"

AxisAlignedBoundingBox2D::AxisAlignedBoundingBox2D(){}
//AxisAlignedBoundingBox2D::AxisAlignedBoundingBox2D(float v) { vmax = v; vmin = v; }
AxisAlignedBoundingBox2D::AxisAlignedBoundingBox2D(vec2 v) : vmin(v), vmax(v) {}
AxisAlignedBoundingBox2D::AxisAlignedBoundingBox2D(vec2 min, vec2 max) : vmin(min), vmax(max) { align(); }
AxisAlignedBoundingBox2D::AxisAlignedBoundingBox2D(vec2 min, vec2 max, bool doalign) : vmin(min), vmax(max), doalign(doalign) { align(); }

bool AxisAlignedBoundingBox2D::intersects(AxisAlignedBoundingBox2D const& bb) const {
	return	vmin.x < bb.maxx() &&
			vmax.x > bb.minx() &&
			vmin.y < bb.maxy() &&
			vmax.y > bb.miny();
}

void AxisAlignedBoundingBox2D::rescale(vec2 const& v) {
	if (v.x < vmin.x) vmin.x = v.x;
	else if (v.x > vmax.x) vmax.x = v.x;
	
	if (v.y < vmin.y) vmin.y = v.y;
	else if (v.y > vmax.y) vmax.y = v.y;
}
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::rescaled(vec2 const& v) const { aabb2 bb = *this; bb.rescale(v); return bb; }

void AxisAlignedBoundingBox2D::narrow(AxisAlignedBoundingBox2D const& bb) {
	if (vmin.x < bb.fmin().x) vmin.x = bb.fmin().x;
	if (vmin.y < bb.fmin().y) vmin.y = bb.fmin().y;
	
	if (vmax.x > bb.fmax().x) vmax.x = bb.fmax().x;
	if (vmax.y > bb.fmax().y) vmax.y = bb.fmax().y;
}
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::narrowed(AxisAlignedBoundingBox2D const& bb) const { aabb2 tbb = *this; tbb.narrow(bb); return tbb; }

void AxisAlignedBoundingBox2D::fit(AxisAlignedBoundingBox2D const& bb) {
	*this = fitted(bb);
}

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::fitted(AxisAlignedBoundingBox2D const& bb) {
	vec2 p = bb.size() / size();
	float scale = outl::minfabs(p.x, p.y);
	return (centered() * scale + bb.center());
}

bool AxisAlignedBoundingBox2D::inside(AxisAlignedBoundingBox2D const& bb) const {
	return  vmin.x >= bb.minx() && vmin.y >= bb.miny() &&
			vmax.x <= bb.maxx() && vmax.y <= bb.maxy();
}

bool AxisAlignedBoundingBox2D::pointInside(vec2 const& v) const {
	return	v.x >= vmin.x && v.x <= vmax.x &&
			v.y >= vmin.y && v.y <= vmax.y;
}

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator+(vec2 const& v) const { return aabb2(vmin + v, vmax + v); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator-(vec2 const& v) const { return aabb2(vmin - v, vmax - v); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator*(vec2 const& v) const { return aabb2(vmin * v, vmax * v); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator/(vec2 const& v) const { return aabb2(vmin / v, vmax / v); }

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator+(AxisAlignedBoundingBox2D const& bb) const { return aabb2(vmin + bb.fmin(), vmax + bb.fmax()); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator-(AxisAlignedBoundingBox2D const& bb) const { return aabb2(vmin - bb.fmin(), vmax - bb.fmax()); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator*(AxisAlignedBoundingBox2D const& bb) const { return aabb2(vmin * bb.fmin(), vmax * bb.fmax()); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator/(AxisAlignedBoundingBox2D const& bb) const { return aabb2(vmin / bb.fmin(), vmax / bb.fmax()); }

AxisAlignedBoundingBox2D operator+ (vec2 const& v, AxisAlignedBoundingBox2D const& bb) { return { v + bb.fmin(), v + bb.fmax() }; }
AxisAlignedBoundingBox2D operator- (vec2 const& v, AxisAlignedBoundingBox2D const& bb) { return { v - bb.fmin(), v - bb.fmax() }; }
AxisAlignedBoundingBox2D operator* (vec2 const& v, AxisAlignedBoundingBox2D const& bb) { return { v * bb.fmin(), v * bb.fmax() }; }
AxisAlignedBoundingBox2D operator/ (vec2 const& v, AxisAlignedBoundingBox2D const& bb) { return { v / bb.fmin(), v / bb.fmax() }; }

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator+=(vec2 const& v) { vmin += v; vmax += v; return *this; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator-=(vec2 const& v) { vmin -= v; vmax -= v; return *this; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator*=(vec2 const& v) { vmin *= v; vmax *= v; return *this; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator/=(vec2 const& v) { vmin /= v; vmax /= v; return *this; }

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator+=(AxisAlignedBoundingBox2D const& bb) { vmin += bb.fmin(); vmax += bb.fmax(); return *this; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator-=(AxisAlignedBoundingBox2D const& bb) { vmin -= bb.fmin(); vmax -= bb.fmax(); return *this; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator*=(AxisAlignedBoundingBox2D const& bb) { vmin *= bb.fmin(); vmax *= bb.fmax(); return *this; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::operator/=(AxisAlignedBoundingBox2D const& bb) { vmin /= bb.fmin(); vmax /= bb.fmax(); return *this; }

bool AxisAlignedBoundingBox2D::operator==(float f) const { return vmin == f && vmax == f; }
bool AxisAlignedBoundingBox2D::operator!=(float f) const { return vmin != f && vmax != f; }

bool AxisAlignedBoundingBox2D::operator==(AxisAlignedBoundingBox2D const& f) const { return vmin == f.fmin() && vmax == f.fmax(); }
bool AxisAlignedBoundingBox2D::operator!=(AxisAlignedBoundingBox2D const& f) const { return vmin != f.fmin() && vmax != f.fmax(); }

void AxisAlignedBoundingBox2D::align() {
	if (!doalign) return;

	if (vmin.x > vmax.x) std::swap(vmin.x, vmax.x);
	if (vmin.y > vmax.y) std::swap(vmin.y, vmax.y);
}

void AxisAlignedBoundingBox2D::fmaxr(vec2 max) { this->vmax = max; align(); }
vec2 AxisAlignedBoundingBox2D::fmax() const { return vmax; }

void AxisAlignedBoundingBox2D::fminr(vec2 min) { this->vmin = min; align(); }
vec2 AxisAlignedBoundingBox2D::fmin() const { return vmin; }

vec2 AxisAlignedBoundingBox2D::center() const { return (vmin + vmax) / 2.f; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::centered() const { return *this - center(); }

float AxisAlignedBoundingBox2D::minx() const { return vmin.x; }
float AxisAlignedBoundingBox2D::miny() const { return vmin.y; }

float AxisAlignedBoundingBox2D::maxx() const { return vmax.x; }
float AxisAlignedBoundingBox2D::maxy() const { return vmax.y; }

void AxisAlignedBoundingBox2D::maxxr(float f) { vmax.x = f; }
void AxisAlignedBoundingBox2D::maxyr(float f) { vmax.y = f; }

void AxisAlignedBoundingBox2D::minxr(float f) { vmin.x = f; }
void AxisAlignedBoundingBox2D::minyr(float f) { vmin.y = f; }

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::maxx(float f) const		{ return aabb2{ vmin, {f, vmax.y} }; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::maxy(float f) const		{ return aabb2{ vmin, {vmax.x, f} }; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::minx(float f) const		{ return aabb2{ { f, vmin.y }, vmax }; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::miny(float f) const		{ return aabb2{ { vmin.x, f }, vmax }; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::fmax(vec2 vmax) const	{ return aabb2{ vmin, vmax }; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::fmin(vec2 vmin) const	{ return aabb2{ vmin, vmax }; }

vec2 AxisAlignedBoundingBox2D::get(int i) const {
	switch (i) {
		case 0: return {vmin.x, vmin.y};
		case 1: return {vmin.x, vmax.y};
		case 2: return {vmax.x, vmax.y};
		case 3: return {vmax.x, vmin.y};
	}

	return 0.f;
}

float AxisAlignedBoundingBox2D::getSingle(int i) const {
	return i == 0 ? vmin.x : i == 1 ? vmin.y : i == 2 ? vmax.x : i == 3 ? vmax.y : 0.f;
}
 
vec2 AxisAlignedBoundingBox2D::operator[](int const& i) const { return get(i); }

std::array<vec2, 4> AxisAlignedBoundingBox2D::toVerts() const { return { get(0), get(1), get(2), get(3) }; }

vec2 AxisAlignedBoundingBox2D::edgeCenter(int i) const {
	return ((*this)[i] + (*this)[(i+1)%4])/2.f;
}

vec2 AxisAlignedBoundingBox2D::size() const { return vmax - vmin; }
float AxisAlignedBoundingBox2D::sizex() const { return vmax.x - vmin.x; }
float AxisAlignedBoundingBox2D::sizey() const { return vmax.y - vmin.y; }

void AxisAlignedBoundingBox2D::scale(vec2 const& scale) { *this = scaled(scale); }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::scaled(vec2 const& scale) const { return centered()*scale + center(); }

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::movedIn(vec2 const& delta) const		{ return { vmin + delta, vmax - delta }; }
AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::movedOut(vec2 const& delta) const	{ return { vmin - delta, vmax + delta }; }

AxisAlignedBoundingBox2D AxisAlignedBoundingBox2D::forcedIn(AxisAlignedBoundingBox2D const& bb) const{
	if (inside(bb)) return *this; //just return if it's already inside
	if (size() > bb.size()) return bb;

	aabb2 resbb = *this;
	if (sizex() > bb.sizex()) resbb = resbb.scaled(vec2(bb.sizex()/sizex() , 1.f));
	if (sizey() > bb.sizey()) resbb = resbb.scaled(vec2(1.f, bb.sizey()/sizey()));

	//translate along x axis if necessary
	bool xtrasl = false;
	if (resbb.minx() < bb.minx()) { resbb += vec2x(bb.minx() - resbb.minx()); xtrasl = true; }
	if (!xtrasl && resbb.maxx() > bb.maxx()) { resbb += vec2x(bb.maxx() - resbb.maxx()); xtrasl = true; }

	bool ytrasl = false;
	if (resbb.miny() < bb.fmin().y) { resbb += vec2y(bb.miny() - resbb.miny()); ytrasl = true; }
	if (!ytrasl && resbb.maxy() > bb.maxy()) { resbb += vec2y(bb.maxy() - resbb.maxy()); ytrasl = true; }

	return resbb;
}


std::string AxisAlignedBoundingBox2D::toString() const {
	std::stringstream ss;
	ss << "(" << vmin << ", " << vmax << ")";
	return ss.str();
}
std::ostream& operator<<(std::ostream& os, AxisAlignedBoundingBox2D const& bb) { return os << bb.toString(); }
std::wostream& operator<<(std::wostream& os, AxisAlignedBoundingBox2D const& bb) { return os << stru::towstr(bb.toString()); }

AxisAlignedBoundingBox2D aabb2s(vec2 const& size) { return { -size/2.f, size/2.f }; }
