#include <parrlib/constants.h>

#include <parrlib/Vector2f.h>

namespace constants {
	float textSizeThreshold = 0.03f;
	const double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;

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