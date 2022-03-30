#pragma once

#include <vector>

#include "math/vector2f.h"
#include "math/vector4f.h"
#include "math/matrix3f.h"
#include "math/matrix4f.h"

#include "shader.h"

#include "framebuffer.h"

namespace prb {
#ifndef DISABLE_VERTBATCH_UTIL
	typedef std::vector<float> vertBatch;
	inline std::vector<float> operator+(std::vector<float> const& a, std::vector<float> const& b) {
		std::vector<float> res = a; res.reserve(a.size() + b.size()); res.insert(res.end(), b.begin(), b.end()); return res;
	}
	inline void operator+=(std::vector<float>& a, std::vector<float> const& b) {
		a.insert(a.end(), b.begin(), b.end());
	}
	struct ttb { size_t from = 0, to = 0; };
	inline vertBatch operator/(vertBatch const& a, ttb tb) { vertBatch res; res.insert(res.end(), a.begin() + tb.from, a.begin() + tb.to); return res; }
	inline vertBatch interleaveVB(vertBatch const& a, size_t dataTypeA, vertBatch const& b, size_t dataTypeB) { 
		vertBatch res; res.reserve(a.size() + b.size());
		
		size_t aIdx = 0, bIdx = 0;
		while (aIdx < a.size() || bIdx < b.size()) {
			if (aIdx < a.size()) { for(int i = aIdx; i < aIdx+dataTypeA; i++) res.push_back(a[i]);	aIdx += dataTypeA; }
			if (bIdx < b.size()) { for(int i = bIdx; i < bIdx+dataTypeB; i++) res.push_back(b[i]);	bIdx += dataTypeB; }
			//if (aIdx < a.size()) { res += a / ttb{ aIdx, aIdx + dataTypeA }; aIdx += dataTypeA; }
			//if (bIdx < b.size()) { res += b / ttb{ bIdx, bIdx + dataTypeB }; bIdx += dataTypeB; }
		}

		return res;
	}
#endif

	class Texture;

	namespace Util {

		extern bool useDefShader;
		extern Shader defShader;
		extern Shader defTexShader;

		void init();

		extern std::vector<mat3> mStack; //matrix stack
		void pushMatrix();
		void pushMatrix(const mat3& modifier);
		void pushReset();
		void popMatrix();
		mat3 getTopMatrix();
		mat3 getTopMatrixAspectInv();
		mat3 getTopMatrixInvAspect();
		mat3 getTopMatrixOrthoInverted();

		mat3 getAspectOrthoX();
		mat3 getAspectOrthoY();
		mat3 getMinAspectOrtho();
		mat3 getMaxAspectOrtho();
		mat3 getAspectOrtho();

		mat3 fromNdc();
		mat3 toNdc();

		mat3 fromNdcAspect();
		mat3 toNdcAspect();

		mat3 fromNdcAspectInv();
		mat3 toNdcAspectInv();

		aabb2 getResbbNDCAspect();

		void translate(vec2 v);
		void rotate(float angle);
		void scale(vec2 v);
		void ortho(float left, float right, float bottom, float top);
		void lookAt(vec2 eye, vec2 pos);
		void multMatrix(const mat3& m);
		void resetMatrix();

		mat3 getQuadrantMatrix(int idx, int nPerRow);

		struct utilDrawState {
			bool outline;
			float strokeWidth;
			vec4 strokeColor = vc4::white;
			vec4 color = vc4::white;

			bool batching = true;
		};
		extern utilDrawState drawState;

		void setColor(vec4 color);
		vec4 getColor();

		void batchTris(std::vector<float>& res, std::vector<float> const& verts);
		void batchShapeConvex(std::vector<float>& res, std::vector<vec2> const& verts);
		void batchShape(std::vector<float>& res, std::vector<vec2> const& verts);
		void batchQuad(std::vector<float>& res, aabb2 quad, const mat3 mat = 1.f);
		//void batchTexture(std::vector<float>& res, const mat3 mat = 1.f);
		void batchCircle(std::vector<float>& res, mat3 mat, const int detail = 40);
		void batchFancyLine(std::vector<float>& res, const std::vector<vec2>& ps, float width, bool loop, const int& style = 0);

		std::vector<float> batchTris(std::vector<float> verts);
		std::vector<float> batchTris(std::vector<vec2> const& verts);
		std::vector<float> batchShapeConvex(std::vector<vec2> const& verts);
		std::vector<float> batchShape(std::vector<vec2> const& verts);
		std::vector<float> batchQuad(aabb2 quad, const mat3 mat = 1.f);
		//std::vector<float> batchTexture(const mat3 mat = 1.f);
		std::vector<float> batchCircle(mat3 mat, const int detail = 40);
		std::vector<float> batchFancyLine(const std::vector<vec2>& ps, float width, bool loop, const int& style = 0);

		void drawTrisNoMatStack(std::vector<float> const& verts);
		void drawTris(std::vector<float> const& verts);
		void drawTris(std::vector<vec2> const& verts);
		void drawShapeConvex(std::vector<vec2> const& verts);
		void drawShape(std::vector<vec2> const& verts);
		void drawShape(std::vector<vec2> verts, mat3 mat);
		void drawPoly(std::vector<vec2> verts);
		void drawQuad(vec2 start, vec2 size, const mat3 mat = 1.f);
		void drawQuad(aabb2 quad, const mat3 mat = 1.f);
		void drawCircle(mat3 mat, const int detail = 40);

		// vertex layout should be { x,y, colorR, colorG, colorB, colorA }
		void drawFancyLine(const std::vector<vec2>& ps, float width, bool loop, const int& style = 0);

		void drawTexture(std::vector<float> const& verts);
		void drawTexture(Texture const& t, std::vector<float> const& verts);
		void drawTexture(const mat3 mat = 1.f);


		std::vector<float> batchTexture(mat3 const& tr);
		std::vector<float> batchTextureSub(mat3 const& tr, aabb2 const& coord, aabb2 const& tx);
		std::vector<float> batchTextureSub(mat3 const& tr, aabb2 const& region);

		void bindTexture(Texture const& t);
		void drawTexture(Texture const& t, const mat3 mat = 1.f);

		void bindFrameBuffer(FrameBuffer& fb);
		void drawFrameBuffer(FrameBuffer& fb, std::vector<float> const& verts);
		void drawFrameBuffer(FrameBuffer& fb, const mat3 mat);

		std::vector<vec2> batchToShape(std::vector<float> const& batch);
	}

	namespace util = Util;
	namespace utl = Util;
}
