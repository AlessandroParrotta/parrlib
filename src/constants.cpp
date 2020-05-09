#include <parrlib/constants.h>

#include <parrlib/Vector2f.h>

#include <cmath>

namespace constants {
	float textSizeThreshold = 0.03f;

	vec2 dres = 0.f;
	void res(vec2 r) { dres = r; }

	float resx() { return dres.x; }
	float resy() { return dres.y; }
	vec2 res() { return dres; }

	aabb2 resbb() { return { 0.f, res() }; }

	float resaspectx(){ return dres.x / dres.y; }
	float resaspecty(){ return dres.y / dres.x; }
	float resaspect() { return std::fmin(resaspectx(), resaspecty()); }
}