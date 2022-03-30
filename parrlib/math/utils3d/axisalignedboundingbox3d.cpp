#include "AxisAlignedBoundingBox3D.h"

namespace prb {
	void AxisAlignedBoundingBox3D::align() {
		if (vmin.x > vmax.x) std::swap(vmin.x, vmax.x);
		if (vmin.y > vmax.y) std::swap(vmin.y, vmax.y);
		if (vmin.z > vmax.z) std::swap(vmin.z, vmax.z);
	}

	AxisAlignedBoundingBox3D::AxisAlignedBoundingBox3D() {}
	AxisAlignedBoundingBox3D::AxisAlignedBoundingBox3D(vec3 vmin, vec3 vmax) : vmin(vmin), vmax(vmax) { if (doalign) align(); }


	float AxisAlignedBoundingBox3D::minx() const { return vmin.x; }
	float AxisAlignedBoundingBox3D::miny() const { return vmin.y; }
	float AxisAlignedBoundingBox3D::minz() const { return vmin.z; }

	float AxisAlignedBoundingBox3D::maxx() const { return vmax.x; }
	float AxisAlignedBoundingBox3D::maxy() const { return vmax.y; }
	float AxisAlignedBoundingBox3D::maxz() const { return vmax.z; }

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::maxx(float f) const { return { vmin, { f, vmax.y, vmax.z } }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::maxy(float f) const { return { vmin, { vmax.x, f, vmax.z } }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::maxz(float f) const { return { vmin, { vmax.x, vmax.y, f } }; }

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::minx(float f) const { return { { f, vmin.y, vmin.z }, vmax }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::miny(float f) const { return { { vmin.x, f, vmin.z }, vmax }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::minz(float f) const { return { { vmin.x, vmin.y, f }, vmax }; }

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::fmax(vec3 vmax) const { return { vmin, vmax }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::fmin(vec3 vmin) const { return { vmin, vmax }; }

	vec3 AxisAlignedBoundingBox3D::get(int i) const {
		switch (i) {
		case 0: return vmin;
		case 1: return { vmin.x, vmax.y, vmin.z };
		case 2: return { vmax.x, vmax.y, vmin.z };
		case 3: return { vmax.x, vmin.y, vmin.z };

		case 4: return { vmin.x, vmin.y, vmax.z };
		case 5: return { vmin.x, vmax.y, vmax.z };
		case 6: return vmax;
		case 7: return { vmax.x, vmin.y, vmax.z };
		}
		return 0.f;
	}
	vec3 AxisAlignedBoundingBox3D::operator[](int i) const { return get(i); }

	vec3 AxisAlignedBoundingBox3D::center() const {
		return (vmin + vmax) / 2.f;
	}

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::centered() const {
		return { vmin - center(), vmax - center() };
	}

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::scaled(vec3 const& scale) const {
		return centered() * scale + center();
	}

	vec3 AxisAlignedBoundingBox3D::edgeCenter(int i) const {
		return (get(i) + get((i + 1) % 8)) / 2.f;
	}

	bool AxisAlignedBoundingBox3D::intersects(AxisAlignedBoundingBox3D const& bb) const {
		return	vmin > bb.fmin() || vmax < bb.fmax();
	}

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator+(vec3 const& v) const { return { vmin + v, vmax + v }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator-(vec3 const& v) const { return { vmin - v, vmax - v }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator*(vec3 const& v) const { return { vmin * v, vmax * v }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator/(vec3 const& v) const { return { vmin / v, vmax / v }; }

	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator+(AxisAlignedBoundingBox3D const& bb) const { return { vmin + bb.fmin(), vmax + bb.fmax() }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator-(AxisAlignedBoundingBox3D const& bb) const { return { vmin - bb.fmin(), vmax - bb.fmax() }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator*(AxisAlignedBoundingBox3D const& bb) const { return { vmin * bb.fmin(), vmax * bb.fmax() }; }
	AxisAlignedBoundingBox3D AxisAlignedBoundingBox3D::operator/(AxisAlignedBoundingBox3D const& bb) const { return { vmin / bb.fmin(), vmax / bb.fmax() }; }

	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator+=(vec3 const& v) { vmin += v; vmax += v; return *this; }
	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator-=(vec3 const& v) { vmin -= v; vmax -= v; return *this; }
	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator*=(vec3 const& v) { vmin *= v; vmax *= v; return *this; }
	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator/=(vec3 const& v) { vmin /= v; vmax /= v; return *this; }

	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator+=(AxisAlignedBoundingBox3D const& bb) { vmin += bb.fmin(); vmax += bb.fmax(); return *this; }
	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator-=(AxisAlignedBoundingBox3D const& bb) { vmin -= bb.fmin(); vmax -= bb.fmax(); return *this; }
	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator*=(AxisAlignedBoundingBox3D const& bb) { vmin *= bb.fmin(); vmax *= bb.fmax(); return *this; }
	AxisAlignedBoundingBox3D& AxisAlignedBoundingBox3D::operator/=(AxisAlignedBoundingBox3D const& bb) { vmin /= bb.fmin(); vmax /= bb.fmax(); return *this; }

	vec3 AxisAlignedBoundingBox3D::fmin() const { return vmin; }
	vec3 AxisAlignedBoundingBox3D::fmax() const { return vmax; }

	bool AxisAlignedBoundingBox3D::inside(AxisAlignedBoundingBox3D const& bb) const {
		return vmin > bb.fmin() && vmax < bb.fmax();
	}

	bool AxisAlignedBoundingBox3D::pointInside(vec3 const& v) const {
		return v > vmin && v < vmax;
	}
}
