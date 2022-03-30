#include "util.h"

#include "constants.h"
#include "math/utils2d/utils2d.h"

#include "debug.h"
#include "vertexbuffer.h"

#include "texture.h"

namespace prb {
	namespace Util {
		bool useDefShader = true;
		Shader defShader;
		Shader defTexShader;
		Shader defFBShader;

		vbuf defVb;
		vbuf texVb;

		Shader uSh;

		void init() {
			uSh = { "shaders/util" };
			defShader = { "shaders/default" };
			defTexShader = { "shaders/texture",
				{
					{"POSITION", "r32g32_float"},
					{"COLOR", "r32g32b32a32_float"},
					{"TEXCOORD", "r32g32_float"}
				}
			};
			defFBShader = { "shaders/framebuffer",
				{
					{"POSITION", "r32g32_float"},
					{"COLOR", "r32g32b32a32_float"},
					{"TEXCOORD", "r32g32_float"}
				}
			};

			defVb = vbuf({ 2, 4 });
			texVb = vbuf({ 2, 4, 2 });

			//deb::msbinfo("util init");
		}

		void setUSHUniform() { uSh.use(); uSh.setUniform("mat", pmat4::toMat4(mStack.back())); uSh.release(); }

		std::vector<mat3> mStack = { 1.f };
		void pushMatrix() { mStack.push_back(mStack.back()); setUSHUniform(); }
		void pushMatrix(const mat3& modifier) { mStack.push_back(mStack.back() * modifier); setUSHUniform(); }
		void pushReset() { mStack.push_back(1.f); setUSHUniform(); }
		void popMatrix() { mStack.pop_back(); setUSHUniform(); }
		mat3 getAspectOrthoX() { return pmat3::orthoAspectX(cst::res()); }
		mat3 getAspectOrthoY() { return pmat3::orthoAspectY(cst::res()); }
		mat3 getMinAspectOrtho() { return pmat3::orthoMinAspect(cst::res()); }
		mat3 getMaxAspectOrtho() { return pmat3::orthoMaxAspect(cst::res()); }
		mat3 getAspectOrtho() { return pmat3::orthoMaxAspect(cst::res()); }

		mat3 getTopMatrix() { return mStack.back(); }
		mat3 getTopMatrixAspectInv() { return getAspectOrtho().inverted() * mStack.back(); }
		mat3 getTopMatrixInvAspect() { return mStack.back().inverted() * getAspectOrtho(); }
		mat3 getTopMatrixOrthoInverted() { return mStack.back().inverted() * getAspectOrtho(); }

		mat3 fromNdc() { return pmat3::fromNdc({ 0.f, cst::res() }); }
		mat3 toNdc() { return pmat3::toNdc({ 0.f, cst::res() }); }

		mat3 fromNdcAspect() { return pmat3::fromNdc({ 0.f, cst::res() }) * getAspectOrtho(); }
		mat3 toNdcAspect() { return getAspectOrtho() * pmat3::toNdc({ 0.f, cst::res() }); }
		mat3 fromNdcAspectInv() { return pmat3::fromNdc({ 0.f, cst::res() }) * getAspectOrtho().inverted(); }
		mat3 toNdcAspectInv() { return getAspectOrtho().inverted() * pmat3::toNdc({ 0.f, cst::res() }); }

		aabb2 getResbbNDCAspect() { return toNdcAspectInv() * cst::resbb(); }

		void translate(vec2 v) { mStack.back() *= pmat3::translate(v); }
		void rotate(float angle) { mStack.back() *= pmat3::rotate(angle); }
		void scale(vec2 v) { mStack.back() *= pmat3::scale(v); }
		void ortho(float left, float right, float bottom, float top) { mStack.back() *= pmat3::ortho(left, right, bottom, top); }
		void lookAt(vec2 eye, vec2 pos) { mStack.back() *= pmat3::lookAt(eye, pos); }
		void multMatrix(const mat3& m) { mStack.back() *= m; }
		void resetMatrix() { mStack.back() = 1.f; }

		utilDrawState drawState;
		void setColor(vec4 color) { drawState.color = color; }
		vec4 getColor() { return drawState.color; }


		//BATCHING FUNCS

		template<typename T>
		std::vector<T> concat(std::vector<T> const& a, std::vector<T> const& b) { std::vector<T> c(a); c.reserve(a.size() + b.size()); c.insert(c.end(), b.begin(), b.end()); return c; }

		template<typename T>
		void concatR(std::vector<T>& a, std::vector<T> const& b) { a.reserve(a.size() + b.size()); a.insert(a.end(), b.begin(), b.end()); }






		mat3 getQuadrantMatrix(int idx, int nPerRow) {
			vec2 pos(idx % nPerRow, floor(idx / nPerRow));

			vec2 perc = pos / (float)nPerRow;

			return pmat3::translate(vec2(-1.f).ny() + perc.ny() * 2.f) * pmat3::scale(1.f / (float)nPerRow) * pmat3::translate(vec2(1.f).ny());
		}

		mat3 getQuadMatrixLine(vec2 p1, vec2 p2, float width) {
			return	pmat3::translate((p1 + p2) / 2.f) *
				pmat3::lookAt(p1, p2) *
				pmat3::scale({ (p1 - p2).magnitude(), width * 2.f });
		}

		float getDegDif(vec2 v0, vec2 v1) {
			if (v0 == v1) return 0.f;

			//vec2 dir = (v1 - v0).normalized();
			//float dot = v0.x * v1.x + v0.y * v1.y;
			//float det = v0.x * v1.y - v0.y * v1.x;
			return ((atan2(v1.y, v1.x) - atan2(v0.y, v0.x)) * 180.f) / 3.14159268f + 180.f; // to degrees
		}

		float getDeg(vec2 v0) { return getDegDif(v2ax::fwv2, v0); }
		inline vec2 getEdge(const vec2& v0, const vec2& v1) { return v1 - v0; }
		inline vec2 getEdgen(const vec2& v0, const vec2& v1) { return (v1 - v0).normalized(); }

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

		std::vector<vec2> getPointsBetweenDegrees(vec2 pos, float cross, float degs, float dege, float radius, int iters) {
			if (iters == 0) return {};
			degs = normalizeDeg(degs); dege = normalizeDeg(dege);

			if (iters < 0) iters = (std::max)(2, (int)(dege > degs ? dege - degs : (360.f - degs) + dege));

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

				if (cross > 0.f) ps.insert(ps.begin(), pos + pmat3::rotate(deg) * (v2ax::fwv2 * radius));
				else			 ps.push_back(pos + pmat3::rotate(deg) * (v2ax::fwv2 * radius));
			}

			return ps;
		}

		//degrees are absolute, i.e they both have to be in reference to (1.f,0.f)(right of the screen) (0 degrees)
		void drawCircleBetweenDegrees(float degs, float dege, float cross, float radius, int iters) {
			setColor(vc4::red);
			drawPoly(getPointsBetweenDegrees(0.f, degs, dege, cross, radius, iters));
		}

		std::vector<vec2> getSemiCircle(vec2 pos, vec2 normal, bool side, float radius, int iters) {
			float a0 = getDeg(normal);
			return getPointsBetweenDegrees(pos, 1.f, a0, a0 + 180.f + (side ? 0.0001f : -0.0001f), radius, iters);
		}

		void batchTris(std::vector<float>& res, std::vector<float> const& verts) {
			if (verts.size() / (2 + 4) < 3) return;
			mat3 tmat = drawState.batching ? mStack.back() : 1.f;

			for (int i = 0; i < verts.size(); i += 6) {
				vec2 p = { verts[i], verts[i + 1] };
				p = tmat * p;
				concatR(res, { p.x, p.y });
			}
		}

		void batchShapeConvex(std::vector<float>& res, std::vector<vec2> const& verts) {
			if (verts.size() < 3) return;
			mat3 tmat = drawState.batching ? mStack.back() : 1.f;

			res.reserve(res.size() + verts.size() * 6 * 2);


			//std::vector<float> rverts; rverts.reserve(verts.size() * (2 + 4));
			vec2 v0 = tmat * verts[0];
			for (int i = 1; i < verts.size() - 1; i++) {
				vec2 v1 = tmat * verts[i];
				vec2 v2 = tmat * verts[i + 1];
				res.insert(res.end(), {
					v0.x,	v0.y,		drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w,
					v1.x,	v1.y,		drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w,
					v2.x,	v2.y,		drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w,
					});
			}
		}

		void batchShape(std::vector<float>& res, std::vector<vec2> const& verts) {
			if (verts.size() < 3) return;

			std::vector<vec2> rverts = utl2::makeClockwise(verts);

			//return batchShapeConvex(verts);

			std::vector<std::vector<vec2>> listNonConvex = { rverts };
			std::vector<std::vector<vec2>> listConvex;

			float accumX = 0.f;
			while (listNonConvex.size() > 0) {
				std::vector<vec2> shape = listNonConvex.back(); listNonConvex.pop_back();
				if (shape.size() < 3) continue;

				if (shape.size() == 3 || utl2::isConvex(shape)) { listConvex.push_back(shape); continue; }

				//DEBUG ONLY, TO SHOW EVERY ITERATION
				//std::vector<vec2> debSh = shape;
				//vec2 xsize = {debSh[0].x, debSh.front().x}; 
				//for (int i = 0; i < debSh.size(); i++) {
				//	if (debSh[i].x < xsize.x) xsize.x = debSh[i].x;
				//	if (debSh[i].x > xsize.y) xsize.y = debSh[i].x;
				//}

				//accumX += (xsize.y - xsize.x) + .5f;
				//for (int i = 0; i < debSh.size(); i++) debSh[i].x += accumX;
				//concatR(fverts, batchFancyLine(debSh, .01f, true, 1));

				// keep on going until you find a vertex that is not visible from the first vertex
				// once you do, go back one, if the previous vertex is adjacent to the first vertex then
				// go forwards until you find a visible vertex, if the next visible vertex is
				// equal to the first vertex ignore this shape

				//int endSeparator = 0;
				//if (utl2::visible(shape, 0, 2)) {
				//	//deb << "directly visible" << shape[0] << " " << shape[2] << "\n";
				//	std::vector<vec2> s1 = { shape[0], shape[1], shape[2] }; listConvex.push_back(s1);
				//	std::vector<vec2> s2; s2.insert(s2.end(), shape.begin() + 2, shape.end()); s2.push_back(shape[0]);
				//	listNonConvex.push_back(s2);
				//}
				//else {
				//	//deb << "NOT directly visible 0 2 " << shape[0] << " " << shape[2] << "\n";
				//	for (int i = 3; i < shape.size(); i++) {
				//		if (utl2::visible(shape, 0, i)) { endSeparator = i; break; }
				//		//deb << "NOT directly visible 0 " << i << " " << shape[0] << " " << shape[i] << "\n";
				//	}

				//	if (endSeparator == 0) continue;
				//	//deb << "now directly visible 0 " << endSeparator << " " << shape[0] << " " << shape[endSeparator] << "\n";

				//	std::vector<vec2> s1; s1.insert(s1.end(), shape.begin()+1, shape.begin() + endSeparator); s1.push_back(shape[endSeparator]);
				//	std::vector<vec2> tri = { shape[0], shape[1], shape[endSeparator] }; listConvex.push_back(tri);
				//	std::vector<vec2> s2; s2.insert(s2.end(), shape.begin() + endSeparator, shape.end()); s2.push_back(shape[0]);
				//	listNonConvex.insert(listNonConvex.end(), { s1, s2 });
				//}

				// BETTER ALGORITHM TO GET LESS TRICKY (THIN) TRIANGLES
				/*
					you start with a starting vertex (t), this vertex is the first vertex that you find
					that has some other vertex (n) not visible from t.
					once you find n, select t as a target.
					y are vertices visible from t, a are vertices adjacent to t (arr[t+1] and arr[t-1] respectively).
					now to the algorithm:
					1. start from the first vertex in the list, check an n from that, adding all y's in the
						way to the final mesh.
						1.1. if no vertex is an n from t, select the next vertex in the list as t.
						1.2. if you can't find an n for every t it means that the shape is convex.
					4. start from n, go on until you find a y.
						4.1 if there is no y there's a bug.
					5. once you find a y, go on until you find another n.
						5.1 if loops back to t select the previous to t.
					6. once you find the n (or the one before t) go back one vertex and that vertex is the last
						vertex.
					7. the final shape will go from t to the first y, then every y until (but not including)
						the n.

					alternative? maybe easier
					-start from t, if t has all visibles go next vertex, if all ts are all visible then error (mesh should be convex)
					-add every visible vertex from t
					-repeat
					-make all the others other meshes
				*/

				//wrapi tmp = shape; 
				//int tmpi = tmp;
				//for (int i = 0; i < shape.size(); i++) { deb << shape[tmp] << " " << shape[tmp+2] << " - " << tmp << " " << (tmp+2) << " " << tmpi << "\n"; tmp++; }

				std::vector<std::vector<vec2>> vecs;
				vecs.push_back(std::vector<vec2>());
				vecs[0].insert(vecs[0].end(), { shape[0], shape[1] });
				bool foundT = false, error = false;
				outl::wrapi t = shape, i = t + 2;
				while (!error && i != t) {
					if (utl2::visible(shape, t, i)) vecs[0].push_back(shape[i]);
					else {
						//generate other mesh
						vecs.push_back(std::vector<vec2>());
						vecs.back().push_back(shape[i - 1]);
						while (!utl2::visible(shape, t, i)) {
							vecs.back().push_back(shape[i]);
							i++;
						}

						vecs.back().push_back(shape[i]);
						vecs[0].push_back(shape[i]);
					}
					i++;

					if (i == t && vecs[0].size() == shape.size()) {
						vecs[0].clear(); t++; i = t + 2;
						vecs[0].insert(vecs[0].end(), { shape[t], shape[t + 1] });
						if (t == 0) error = true;
					}
				}

				//std::vector<std::vector<vec2>> vecs;
				//vecs.push_back(std::vector<vec2>());
				//vecs[0].insert(vecs[0].end(), { shape[0], shape[1] });
				//bool foundT = false, error = false;
				//int t = 0, i = t+2;
				//while (!error && i != t) {
				//	deb << i - 1 << "\n";
				//	if (utl2::visible(shape, t, i)) vecs[0].push_back(shape[i]);
				//	else {
				//		//generate other mesh
				//		vecs.push_back(std::vector<vec2>());
				//		//vecs.back().push_back(shape[i-1]);
				//		vecs.back().push_back(shape[i]);
				//		while (!utl2::visible(shape, t, i)) {
				//			vecs.back().push_back(shape[i]);
				//			i = wrap(i+1, shape.size());
				//		}

				//		vecs.back().push_back(shape[i]);
				//		vecs[0].push_back(shape[i]); 
				//	}
				//	i = wrap(i + 1, shape.size());

				//	debss << i << " " << t << " " << (i!=t)  << "\n";
				//	

				//	if(i==t && vecs[0].size() == shape.size()) {
				//		vecs[0].clear(); t = wrap(t + 1, shape.size()); i = wrap(t + 2, shape.size());
				//		vecs[0].insert(vecs[0].end(), { shape[t], shape[wrap(t + 1, shape.size())] });
				//		if (t == 0) error = true;
				//	}
				//	deb << t << " " << i << "\n";
				//}

				for (int i = 0; i < vecs.size(); i++) listNonConvex.push_back(vecs[i]);

				//while (!foundT) {
				//	int it = t+2;
				//	while (it != t) {
				//		while (utl2::visible(shape, t, it) && it != t) {
				//			vecs[0].push_back(shape[it]);
				//			it = incwrap(it, shape.size());
				//		}
				//		if (it != t) foundT = true;

				//		it = incwrap(it, shape.size());
				//	}

				//	if (!foundT) t = incwrap(t, shape.size());
				//}
			}

			//for (int i = 0; i < listConvex.size(); i++) concatR(fverts, batchFancyLine(listConvex[i], .01f, true, 1));
			for (int i = 0; i < listConvex.size(); i++) concatR(res, batchShapeConvex(listConvex[i]));
			//deb << "batched " << listConvex.size() << " convex shapes\n";
		}

		void batchQuad(std::vector<float>& res, aabb2 quad, const mat3 mat) {
			if (drawState.batching) pushMatrix(mat);
			batchShapeConvex(res, {
				quad[0],
				quad[1],
				quad[2],
				quad[3]
				});
			if (drawState.batching) popMatrix();
		}

		//void batchTexture(std::vector<float>& res, const mat3 mat = 1.f) {
		//	mat3 m = drawState.batching ? mStack.back() : 1.f;
		//}

		void batchCirclePriv(std::vector<float>& res, mat3 mat, const int detail) {
			if (detail <= 2) return;
			mat3 tmat = drawState.batching ? mStack.back() * mat : 1.f;

			res.reserve(res.size() + detail * (2 + 4));

			vec2 center = tmat * vec2(0.f);
			for (int i = 0; i < detail; i++) {
				vec2 v0 = tmat * vec2(cosf((float)i / (float)detail * 3.14159268f * 2.f), sinf((float)i / (float)detail * 3.14159268f * 2.f));
				vec2 v1 = tmat * vec2(cosf((float)(i + 1) / (float)detail * 3.14159268f * 2.f), sinf((float)(i + 1) / (float)detail * 3.14159268f * 2.f));
				res.insert(res.end(),
					{
						center.x, center.y,		drawState.color.x, drawState.color.y, drawState.color.z, drawState.color.w,
						v1.x, v1.y,				drawState.color.x, drawState.color.y, drawState.color.z, drawState.color.w,
						v0.x, v0.y,				drawState.color.x, drawState.color.y, drawState.color.z, drawState.color.w,
					}
				);
			}
		}
		void batchCircle(std::vector<float>& res, mat3 mat, const int detail) {
			if (drawState.outline) { vec4 oldColor = getColor(); setColor(drawState.strokeColor); batchCirclePriv(res, mat * pmat3::scale(1.f + drawState.strokeWidth * 4.f), detail); setColor(oldColor); }
			batchCirclePriv(res, mat, detail);
		}

		void batchFancyLinePriv(std::vector<float>& res, const std::vector<vec2>& ps, float width, bool loop, const int& style) {
			if (ps.size() < 2) return;
			if (width <= 0.f) return;

			if (loop && ps.size() < 3) loop = false;
			std::vector<mat3> mats;
			mats.reserve(ps.size() - (loop ? 0 : 1));
			for (int i = 0; i < ps.size() - (loop ? 0 : 1); i++) mats.push_back(getQuadMatrixLine(ps[i], ps[(i + 1) % ps.size()], width));

			switch (style) {
			case 0:
			{
				if (!loop) {
					batchShapeConvex(res, getSemiCircle(mats.front() * vec2(-.5f, .0f), getEdgen(mats.front() * vec2(0.f, .5f), mats.front() * vec2(0.f, -.5f)), false, width, -1));
					batchShapeConvex(res, getSemiCircle(mats.back() * vec2(.5f, .0f), getEdgen(mats.back() * vec2(0.f, -.5f), mats.back() * vec2(0.f, .5f)), false, width, -1));
				}

				for (int i = 0; i < ps.size() - (loop ? 0 : 1); i++) {

					batchQuad(res, { -.5f, .5f }, mats[i]);

					if (i < ps.size() - (loop ? 0 : 2)) {

						float face = (getEdge(ps[i], ps[(i + 1) % ps.size()]).perp().dot(-getEdge(ps[(i + 1) % ps.size()], ps[(i + 2) % ps.size()]))) > 0.f ? 1.f : -1.f;

						std::vector<vec2> pst = { //3 main points that make the circle, then iterate to create it
							mats[i] * vec2(.5f,0.f),
							mats[i] * vec2(.5f,.5f * face),
							mats[(i + 1) % mats.size()] * vec2(-.5f,.5f * face)
						};

						vec2 edge0 = -getEdge(pst[0], pst[1]), edge1 = -getEdge(pst[0], pst[2]);
						float a0 = getDeg(edge0), a1 = getDeg(edge1);
						std::vector<vec2> fps = getPointsBetweenDegrees(ps[(i + 1) % ps.size()], edge0.cross(edge1), a0, a1, edge0.magnitude(), -1);

						//if (edge0.cross(edge1) > 0.f) {
						//	std::vector<vec2> inv; inv.reserve(fps.size());
						//	for (int i = 0; i < fps.size(); i++) {
						//		inv.push_back(fps[fps.size() - 1 - i]);
						//	}
						//	fps = inv;
						//}

						batchShapeConvex(res, fps);
					}
				}
			}
			break;
			case 1:
				for (int i = 0; i < ps.size() - (loop ? 0 : 1); i++) {

					batchQuad(res, { -.5f, .5f }, mats[i]);

					if (i < ps.size() - (loop ? 0 : 2)) {

						float cross = (ps[(i + 1) % ps.size()] - ps[i]).cross(-(ps[(i + 2) % ps.size()] - ps[(i + 1) % ps.size()]));
						float face = ((ps[(i + 1) % ps.size()] - ps[i]).perp().dot(-(ps[(i + 2) % ps.size()] - ps[(i + 1) % ps.size()]))) > 0.f ? 1.f : -1.f;
						if (cross < 0.f) {
							batchShapeConvex(res, {
								mats[(i + 1) % mats.size()] * vec2(-.5f,.5f * face),
								mats[i] * vec2(.5f,.5f * face),
								mats[i] * vec2(.5f,0.f)
								});
						}
						else {
							batchShapeConvex(res, {
								mats[i] * vec2(.5f,0.f),
								mats[i] * vec2(.5f,.5f * face),
								mats[(i + 1) % mats.size()] * vec2(-.5f,.5f * face)
								});
						}
					}
				}
				break;
			case 2:
				for (int i = 0; i < ps.size() - (loop ? 0 : 1); i++) {

					batchQuad(res, { -.5f, .5f }, mats[i]);

					if (i < ps.size() - (loop ? 0 : 2)) {

						float cross = (ps[(i + 1) % ps.size()] - ps[i]).cross(-(ps[(i + 2) % ps.size()] - ps[(i + 1) % ps.size()]));
						float face = ((ps[(i + 1) % ps.size()] - ps[i]).perp().dot(-(ps[(i + 2) % ps.size()] - ps[(i + 1) % ps.size()]))) > 0.f ? 1.f : -1.f;

						std::vector<vec2> pst = {
							mats[i] * vec2(.5f,0.f),
							mats[i] * vec2(.5f,.5f * face),
							mats[(i + 1) % mats.size()] * vec2(-.5f,.5f * face)
						};


						vec2 mid = (pst[1] + pst[2]) / 2.f;
						vec2 toCenter = getEdge(mid, pst[0]);

						vec2 third = mid - toCenter;

						vec2 p = getPointLinesMeet(
							mats[i] * vec2(.5f, .5f * face),
							mats[i] * vec2(-.5f, .5f * face),

							mats[(i + 1) % mats.size()] * vec2(.5f, .5f * face),
							mats[(i + 1) % mats.size()] * vec2(-.5f, .5f * face)
						);

						if ((p - pst[0]).magnitude() > (third - pst[0]).magnitude())	pst.insert(pst.begin() + 2, third);
						else															pst.insert(pst.begin() + 2, p);

						if (cross < 0.f) {
							std::vector<vec2> inv = { pst[3], pst[2], pst[1], pst[0] };
							pst = inv;
						}

						batchShapeConvex(res, pst);
					}
				}
				break;
			}
		}
		void batchFancyLine(std::vector<float>& res, const std::vector<vec2>& ps, float width, bool loop, const int& style) {
			if (!drawState.batching) pushReset();
			bool drs = drawState.batching; drawState.batching = true;

			if (drawState.outline) { vec4 oldColor = getColor(); setColor(drawState.strokeColor); batchFancyLinePriv(res, ps, width + drawState.strokeWidth, loop, style); setColor(oldColor); }
			batchFancyLinePriv(res, ps, width, loop, style);

			drawState.batching = drs;
			if (!drawState.batching) popMatrix();
		}

		std::vector<float> batchTris(std::vector<float> verts) { std::vector<float> res; batchTris(res, verts);		return res; }
		std::vector<float> batchTris(std::vector<vec2> const& verts) { std::vector<float> res; batchShapeConvex(res, verts);	return res; }
		std::vector<float> batchShapeConvex(std::vector<vec2> const& verts) { std::vector<float> res; batchShapeConvex(res, verts); return res; }
		std::vector<float> batchShape(std::vector<vec2> const& verts) { std::vector<float> res; batchShape(res, verts);		return res; }
		std::vector<float> batchQuad(aabb2 quad, const mat3 mat) { std::vector<float> res; batchQuad(res, quad, mat);	return res; }
		std::vector<float> batchCircle(mat3 mat, const int detail) { std::vector<float> res; batchCircle(res, mat, detail); return res; }
		std::vector<float> batchFancyLine(const std::vector<vec2>& ps, float width, bool loop, const int& style) { std::vector<float> res; batchFancyLine(res, ps, width, loop, style); return res; }


		void drawSemiCircle(vec2 normal, bool side, float radius, int iters) {
			setColor(vc4::red);
			drawPoly(getSemiCircle(0.f, normal, side, radius, iters));
		}


		void drawTrisNoMatStack(std::vector<float> const& verts) {
			if (verts.size() / (2 + 4) < 3) return;

			if (useDefShader) uSh.use();
			//if (useDefShader) defShader.use();

			//vbuf vb(verts, sizeof(float)*(2+4), 0);
			//defVb.use();
			defVb.setData(verts);
			defVb.draw();

			//deb::msbinfo("draw tris no mat stack");

			//vb.release();
			//vb.dispose();
		}

		void drawTrisNoMatStackReset(std::vector<float> const& verts) { drawTrisNoMatStack(verts); }

		// draw triangles, expected layout for every vertex is:
		// { posX, posY, colorR, colorG, colorB, colorA }
		// vertices should be directly one after the other
		// note that dxutil::color does not affect this function
		void drawTris(std::vector<float> const& verts) {
			if (verts.size() / (2 + 4) < 3) return;

			//if (useDefShader) {
			//	defCB.use();
			//}

			drawTrisNoMatStack(verts);
		}

		// draw triangles using vec2 vertices
		// use dxutil::color to set the color of all triangles at once
		void drawTris(std::vector<vec2> const& verts) { drawState.batching = false; drawTris(batchTris(verts));						drawState.batching = true; }
		void drawShapeConvex(std::vector<vec2> const& verts) { drawState.batching = false; drawTris(batchShapeConvex(verts));				drawState.batching = true; }
		void drawShape(std::vector<vec2> const& verts) { drawState.batching = false; drawTris(batchShape(verts));					drawState.batching = true; }
		void drawShape(std::vector<vec2> verts, mat3 mat) { pushMatrix(mat);	drawState.batching = false; drawShape(verts);								drawState.batching = true; popMatrix(); }
		void drawPoly(std::vector<vec2> verts) { drawState.batching = false; drawShape(verts, 1.f);							drawState.batching = true; }
		void drawQuad(vec2 start, vec2 size, const mat3 mat) { pushMatrix(mat);	drawState.batching = false; drawTris(batchQuad({ start, size }));			drawState.batching = true; popMatrix(); }
		void drawQuad(aabb2 quad, const mat3 mat) { pushMatrix(mat);	drawState.batching = false; drawTris(batchQuad(quad));						drawState.batching = true; popMatrix(); }
		void drawCircle(mat3 mat, const int detail) { pushMatrix(mat);	drawState.batching = false; drawTris(batchCircle(mat, detail));				drawState.batching = true; popMatrix(); }

		//style = 0: rounded corners, style = 1: rectangle corners, style = 2: spike corners
		void drawFancyLine(const std::vector<vec2>& ps, float width, bool loop, const int& style) { drawState.batching = false; drawTris(batchFancyLine(ps, width, loop, style)); drawState.batching = true; }


		std::vector<float> batchTexture(mat3 const& tr) {
			mat3 fm = getTopMatrix() * tr;

			aabb2 tx = aabb2{ 0.f,1.f };
			aabb2 qd = fm * aabb2{ -1.f,1.f };

			//deb::rtss << tx[0] << " " << tx[1] << " " << tx[2] << " " << tx[3] << "\n";

			//vbuf vb({
			//	qd[0].x, qd[0].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[1].x,tx[1].y,
			//	qd[1].x, qd[1].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[0].x,tx[0].y,
			//	qd[2].x, qd[2].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[3].x,tx[3].y,
			//	
			//	qd[0].x, qd[0].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[1].x,tx[1].y,
			//	qd[2].x, qd[2].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[3].x,tx[3].y,
			//	qd[3].x, qd[3].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[2].x,tx[2].y,
			//}, sizeof(float)*(2+4+2), 0);

			return {
				qd[0].x, qd[0].y,  drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w, tx[0].x,tx[0].y,
				qd[1].x, qd[1].y,  drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w, tx[1].x,tx[1].y,
				qd[2].x, qd[2].y,  drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w, tx[2].x,tx[2].y,

				qd[0].x, qd[0].y,  drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w, tx[0].x,tx[0].y,
				qd[2].x, qd[2].y,  drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w, tx[2].x,tx[2].y,
				qd[3].x, qd[3].y,  drawState.color.x,drawState.color.y,drawState.color.z,drawState.color.w, tx[3].x,tx[3].y,
			};
		}

		std::vector<float> batchTextureSub(mat3 const& tr, aabb2 const& coord, aabb2 const& tx) {
			mat3 fm = getTopMatrix() * tr;

			//aabb2 tx = tx;
			aabb2 qd = fm * coord;

			return {
				qd[0].x, qd[0].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[0].x,tx[0].y,
				qd[1].x, qd[1].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[1].x,tx[1].y,
				qd[2].x, qd[2].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[2].x,tx[2].y,

				qd[0].x, qd[0].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[0].x,tx[0].y,
				qd[2].x, qd[2].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[2].x,tx[2].y,
				qd[3].x, qd[3].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[3].x,tx[3].y,
			};
		}
		std::vector<float> batchTextureSub(mat3 const& tr, aabb2 const& region) { return batchTextureSub(tr, { -1.f,1.f }, region); }

		void drawTexture(std::vector<float> const& verts) {
			texVb.setData(verts);

			defTexShader.use();

			texVb.draw();

			defTexShader.release();
		}

		void drawTexture(const mat3 mat) { drawTexture(batchTexture(mat)); }
		void bindTexture(Texture const& t) { t.bind(); }
		void drawTexture(Texture const& t, std::vector<float> const& verts) { bindTexture(t); drawTexture(verts); }
		void drawTexture(Texture const& t, const mat3 mat) { bindTexture(t); drawTexture(mat); }

		void drawFrameBuffer(std::vector<float> const& verts) {
			texVb.setData(verts);

			defFBShader.use();

			texVb.draw();

			defFBShader.release();
		}

		void drawFrameBuffer(mat3 const& mat) {
			mat3 fm = getTopMatrix() * mat;

			aabb2 tx = aabb2{ 0.f,1.f };
			aabb2 qd = fm * aabb2{ -1.f,1.f };

			drawFrameBuffer({
				qd[0].x, qd[0].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[0].x,tx[0].y,
				qd[1].x, qd[1].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[1].x,tx[1].y,
				qd[2].x, qd[2].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[2].x,tx[2].y,

				qd[0].x, qd[0].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[0].x,tx[0].y,
				qd[2].x, qd[2].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[2].x,tx[2].y,
				qd[3].x, qd[3].y,  drawState.color.x,drawState.color.x,drawState.color.x,drawState.color.x, tx[3].x,tx[3].y,
				});
		}

		void bindFrameBuffer(FrameBuffer& fb) { fb.bind(); }
		void drawFrameBuffer(FrameBuffer& fb, std::vector<float> const& verts) { fb.bindTexture(); drawFrameBuffer(verts); fb.unbindTexture(); }
		void drawFrameBuffer(FrameBuffer& fb, const mat3 mat) { fb.bindTexture(); drawFrameBuffer(mat); fb.unbindTexture(); }
		
		unsigned int commonVerts(int v, std::vector<float> const& mesh) {
			unsigned int res = 0;
			for (int i = 0; i < mesh.size(); i += 6) {
				if (v != i) {
					res += vec2(mesh[i], mesh[i + 1]) == vec2(mesh[v], mesh[v + 1]) ? 1 : 0;
				}
			}
			return res;
		}

		std::vector<std::array<vec2, 3>> batchToShapeTris(std::vector<float> const& batch) {
			std::vector<std::array<vec2, 3>> res; res.reserve(batch.size() / 6 / 3);
			for (int i = 0; i < batch.size(); i += 6 * 3) {
				if (i + 13 >= batch.size()) break;

				res.push_back(std::array<vec2, 3>{
					vec2{ batch[i + 0], batch[i+1] },
					vec2{ batch[i + 6], batch[i+7] },
					vec2{ batch[i + 12], batch[i+13] }
				});
			}
			return res;
		}

		bool contains(vec2 const& p, std::vector<std::array<vec2, 3>> const& tris) {
			for (int i = 0; i < tris.size(); i += 6 * 3) {

				if (utl2::contains(p, std::vector<vec2>{
					tris[i][0],
					tris[i][1],
					tris[i][2],
				})) return true;
			}

			return false;
		}

		bool contains(vec2 const& p, std::vector<float> const& batch) {
			for (int i = 0; i < batch.size(); i += 6*3) {
				if (i + 13 >= batch.size()) break;

				//deb::pr("check contains ",p,"\n\t", 
				//	vec2{ batch[i + 0], batch[i + 1] }, "\n\t",
				//	vec2{ batch[i + 6], batch[i + 7] }, "\n\t",
				//	vec2{ batch[i + 12], batch[i + 13] }, "\n"
				//);

				if (utl2::contains(p, std::vector<vec2>{
					{ batch[i + 0], batch[i + 1] },
					{ batch[i + 6], batch[i + 7] },
					{ batch[i + 12], batch[i + 13] },
				})) { /*deb::pr("\n\t### is contained ###\n\n");*/ return true; }
			}

			//deb::pr("\n\t!!!!!!!!! is NOT contained !!!!!!!!!\n\n");

			return false;
		}

		bool isOuterEdge(int i1, int i2, std::vector<float> const& batch) {
			utl2::Line edge = { { batch[i1], batch[i1+1] }, { batch[i2], batch[i2+1] } };
			//deb::pr("edge ", edge, " ### ", (edge.center() + edge.dir().perp() * .1f), " --- ", (edge.center() + -edge.dir().perp() * .1f), "\n");
			return
				!contains(edge.center() + edge.dir().perp() * .1f, batch) ||
				!contains(edge.center() + -edge.dir().perp() * .1f, batch);
			
		}

		std::vector<vec2> batchToShape(std::vector<float> const& batch) {
			std::vector<vec2> res;

			for(int i=0; i<batch.size(); i += 6*3){
				if (i + 13 >= batch.size()) break;

				//deb::pr(i,"\n");

				if (isOuterEdge(i, i+6, batch))		{ /*deb::pr("\t1 contained\n");*/ res.push_back({ batch[i],		batch[i+1] }); }
				if (isOuterEdge(i+6, i+12, batch))	{ /*deb::pr("\t2 contained\n");*/ res.push_back({ batch[i+6],	batch[i+7] }); }
				if (isOuterEdge(i+12, i, batch))	{ /*deb::pr("\t3 contained\n");*/ res.push_back({ batch[i+12],	batch[i+13] }); }
			}
			
			return res;
		}
	}

}
