#include <parrlib/OtherUtil.h>

#include <parrlib/Vector2f.h>
#include <parrlib/Vector3f.h>
#include <parrlib/Vector4f.h>

#include <parrlib/Matrix2f.h>
#include <parrlib/Matrix3f.h>
#include <parrlib/Matrix4f.h>

namespace otherutil {
	int loadInt(std::ifstream& f)				{ int v;			f >> v; return v; }
	float loadFloat(std::ifstream& f)			{ float v;			f >> v; return v; }
	double loadDouble(std::ifstream& f)			{ double v;			f >> v; return v; }
	char loadChar(std::ifstream& f)				{ char v;			f >> v; return v; }
	std::string loadString(std::ifstream& f)	{ std::string v;	f >> v; return v; }

	vec2 loadVec2(std::ifstream& f)				{ return { loadFloat(f), loadFloat(f) }; }
	vec3 loadVec3(std::ifstream& f)				{ return { loadFloat(f), loadFloat(f), loadFloat(f) }; }
	vec4 loadVec4(std::ifstream& f)				{ return { loadFloat(f), loadFloat(f), loadFloat(f), loadFloat(f) }; }

	Matrix2f loadMat2(std::ifstream& f)			{ float mat[2][2]; for (int i = 0; i < 2; i++) { for (int j = 0; j < 2; j++) { mat[i][j] = loadFloat(f); } } return Matrix2f(mat); }
	Matrix3f loadMat3(std::ifstream& f)			{ float mat[3][3]; for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { mat[i][j] = loadFloat(f); } } return Matrix3f(mat); }
	Matrix4f loadMat4(std::ifstream& f)			{ float mat[4][4]; for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) { mat[i][j] = loadFloat(f); } } return Matrix4f(mat); }

	int sign(int const& v) { return v >= 0 ? 1 : -1; }
	float sign(float const& v) { return v >= 0.f ? 1.f : -1.f; }
	double sign(double const& v) { return v >= 0. ? 1. : -1.; }

	float lerp(float v, float target, float speed) { return (target - v) * speed * gtime::deltaTime; }
	double lerp(double v, double target, double speed) { return (target - v) * speed * gtime::deltaTime; }

	void lerpr(float& v, float target, float speed) { v += lerp(v, target, speed); }
	void lerpr(double& v, double target, double speed) { v += lerp(v, target, speed); }
	
	float lerprr(float& v, float target, float speed) { v += lerp(v, target, speed); return v; }
	double lerprr(double& v, double target, double speed) { v += lerp(v, target, speed); return v; }

	vec2 lerp(vec2 const& v, vec2 const& target, float speed) { return (target - v) * speed * gtime::deltaTime; }

	float minfabs(float const& f1, float const& f2) { return std::fabs(f1) < std::fabs(f2) ? f1 : f2; }
	float maxfabs(float const& f1, float const& f2) { return std::fabs(f1) > std::fabs(f2) ? f1 : f2; }

	//float frand() { return (float)rand() / (float)RAND_MAX; }

	//Normalizes any number to an arbitrary range, [-180,180], [0,180] or [0,360]
	//by assuming the range wraps around when going below min or above max 
	//https://stackoverflow.com/questions/1628386/normalise-orientation-between-0-and-360
	double normalizeDegAnyVal(const double value, const double start, const double end)
	{
		const double width = end - start;   // 
		const double offsetValue = value - start;   // value relative to 0

		return (offsetValue - (floor(offsetValue / width) * width)) + start;
		// + start to reset back to start of original range
	}

	float normalizeDeg(float deg) {
		if (deg > 0.f && deg < 360.f) return deg;

		deg = fmod(deg, 360.f);
		if (deg < 0.f) deg += 360.f;

		return deg;
	}

	std::vector<vec2> getPointsBetweenDegrees(vec2 pos, float degs, float dege, float radius, int iters) {
		degs = normalizeDeg(degs); dege = normalizeDeg(dege);

		float fdir = abs(dege - degs) < 180.f ? 1.f : -1.f;

		std::vector<vec2> ps = { pos + 0.f };

		for (int i = 0; i < iters; i++) {
			float perc = (float)i / (float)(iters - 1);

			float deg = 0.f;
			if (fdir > 0.f) deg = degs + (dege - degs) * perc;
			else {
				float invS = normalizeDeg(degs + 180.f);
				float invE = normalizeDeg(dege + 180.f);
				deg = degs + (invE - invS) * perc;
			}

			ps.push_back(pos + pmat3::rotate(deg) * (v2ax::fwv2 * radius));
		}

		return ps;
	}

	float getDegDif(vec2 v0, vec2 v1) {
		if (v0 == v1) return 0.f;
		return ((atan2(v1.y, v1.x) - atan2(v0.y, v0.x)) * 180.f) / cst::PI + 180.f; // to degrees
	}

	float getDeg(vec2 v0) { return getDegDif(v2ax::fwv2, v0); }

	std::vector<vec2> getSemiCircle(vec2 pos, vec2 normal, bool side, float radius, int iters) {
		float a0 = getDeg(normal);
		return getPointsBetweenDegrees(pos, a0, a0 + 180.f + (side ? 0.0001f : -0.0001f), radius, iters);
	}

	vec2 getPointLinesMeet(vec2 l1s, vec2 l1e, vec2 l2s, vec2 l2e) {
		// Line AB represented as a1x + b1y = c1 
		float a1 = l1e.y - l1s.y;
		float b1 = l1s.x - l1e.x;
		float c1 = a1 * (l1s.x) + b1 * (l1s.y);

		// Line CD represented as a2x + b2y = c2 
		float a2 = l2e.y - l2s.y;
		float b2 = l2s.x - l2e.x;
		float c2 = a2 * (l2s.x) + b2 * (l2s.y);

		float determinant = a1 * b2 - a2 * b1;

		if (determinant == 0.f)
		{
			// The lines are parallel. This is simplified 
			// by returning a pair of FLT_MAX 
			return 0.f;
		}
		else
		{
			float x = (b2 * c1 - b1 * c2) / determinant;
			float y = (a1 * c2 - a2 * c1) / determinant;
			return vec2(x, y);
		}
	}

	float clamp(float f, float min, float max) { return std::fminf(std::fmaxf(f, min), max); }
	float clamp(double d, double min, double max) { return std::fmin(std::fmax(d, min), max); }

	int writeToBMP(const std::string& fileName, const std::vector<std::vector<Vector3f>>& pixels) {
		FILE* f;
		unsigned char* img = NULL;

		int w = pixels[0].size(), h = pixels.size();

		int filesize = 54 + 3 * w * h;  //w is your image width, h is image height, both int

		img = (unsigned char*)malloc(3 * w * h);
		memset(img, 0, 3 * w * h);

		int x, y, r, g, b;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				x = j; y = (h - 1) - i;
				r = pixels[i][j].x * 255;
				g = pixels[i][j].y * 255;
				b = pixels[i][j].z * 255;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;
				img[(x + y * w) * 3 + 2] = (unsigned char)(r);
				img[(x + y * w) * 3 + 1] = (unsigned char)(g);
				img[(x + y * w) * 3 + 0] = (unsigned char)(b);
			}
		}

		unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
		unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
		unsigned char bmppad[3] = { 0,0,0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(w);
		bmpinfoheader[5] = (unsigned char)(w >> 8);
		bmpinfoheader[6] = (unsigned char)(w >> 16);
		bmpinfoheader[7] = (unsigned char)(w >> 24);
		bmpinfoheader[8] = (unsigned char)(h);
		bmpinfoheader[9] = (unsigned char)(h >> 8);
		bmpinfoheader[10] = (unsigned char)(h >> 16);
		bmpinfoheader[11] = (unsigned char)(h >> 24);

		fopen_s(&f, fileName.c_str(), "wb");
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i < h; i++) {
			fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
			fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
		}

		free(img);
		fclose(f);

		return 0;
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring toWString(std::string str) {
		std::wstring wide = converter.from_bytes(str);
		return wide;
	}
}
