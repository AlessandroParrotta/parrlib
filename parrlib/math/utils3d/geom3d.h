#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "../matrix4f.h"
#include "../vector4f.h"
//#include "../../util.h"

namespace prb {
	namespace geom3D {
		struct geommesh3;

		geommesh3 fromFileSingle(const char* fileName); //puts the entire scene into one single mesh

		struct geommat { //material
			std::vector<std::string> texpaths; //texture paths
		};

		//temporary structure to hold mesh information in
		struct geommesh3 {
			std::vector<float> pos;		//positions,	3 per position
			std::vector<float> norm;	//normals,		3 per normal
			std::vector<float> col;		//colors,		4 per color
			std::vector<float> uv;		//uv,			2 per uv
			std::vector<unsigned int> idx;		//indexes,		3 per triangle

			geommat mat; //material

			geommesh3() {}

			geommesh3(std::vector<float> const& pos, std::vector<float> const& norm, std::vector<float> const& col, std::vector<float> const& uv, std::vector<unsigned int> const& idx)
				: pos(pos), norm(norm), col(col), uv(uv), idx(idx) {}

			geommesh3(std::vector<vec3> const& pos, std::vector<vec3> const& norm, std::vector<vec4> const& col, std::vector<vec2> const& uv, std::vector<unsigned int> const& idx)
				: idx(idx) {
				this->pos.reserve(pos.size() * 3);
				this->norm.reserve(norm.size() * 3);
				this->col.reserve(col.size() * 4);
				this->uv.reserve(uv.size() * 2);

				for (int i = 0; i < pos.size(); i++) { this->pos.push_back(pos[i].x); this->pos.push_back(pos[i].y); this->pos.push_back(pos[i].z); }
				for (int i = 0; i < norm.size(); i++) { this->norm.push_back(norm[i].x); this->norm.push_back(norm[i].y); this->norm.push_back(norm[i].z); }
				for (int i = 0; i < col.size(); i++) { this->col.push_back(col[i].x); this->col.push_back(col[i].y); this->col.push_back(col[i].z); this->col.push_back(col[i].w); }
				for (int i = 0; i < uv.size(); i++) { this->uv.push_back(uv[i].x); this->uv.push_back(uv[i].y); }
			}

			geommesh3(const char* fileName) { *this = fromFileSingle(fileName); }

			void pushPos(vec3 const& pos) { this->pos.push_back(pos.x);	this->pos.push_back(pos.y);		this->pos.push_back(pos.z); }
			void pushNorm(vec3 const& norm) { this->norm.push_back(norm.x); this->norm.push_back(norm.y);	this->norm.push_back(norm.z); }
			void pushCol(vec4 const& col) { this->col.push_back(col.x);	this->col.push_back(col.y);		this->col.push_back(col.z);  this->col.push_back(col.w); }
			void pushUv(vec2 const& uv) { this->uv.push_back(uv.x);		this->uv.push_back(uv.y); }
			void pushIdx(unsigned int idx) { this->idx.push_back(idx); }

			//note: order of insertion is inverted so that it shifts the values to their correct place without modifying 'i'
			void insertPos(int ti, vec3 const& pos) { int i = ti * 3; this->pos.insert(this->pos.begin() + i, pos.z); this->pos.insert(this->pos.begin() + i, pos.y); this->pos.insert(this->pos.begin() + i, pos.x); }
			void insertNorm(int ti, vec3 const& norm) { int i = ti * 3; this->norm.insert(this->norm.begin() + i, norm.z); this->norm.insert(this->norm.begin() + i, norm.y); this->norm.insert(this->norm.begin() + i, norm.x); }
			void insertCol(int ti, vec4 const& col) { int i = ti * 4; this->col.insert(this->col.begin() + i, col.w); this->col.insert(this->col.begin() + i, col.z); this->col.insert(this->col.begin() + i, col.y); this->col.insert(this->col.begin() + i, col.x); }
			void insertUv(int ti, vec2 const& uv) { int i = ti * 2; this->uv.insert(this->uv.begin() + i, uv.y); this->uv.insert(this->uv.begin() + i, uv.x); }
			void insertIdx(int i, unsigned int idx) { this->idx.insert(this->idx.begin() + i, idx); }

			void pushVert(vec3 const& pos, vec3 const& norm, vec4 const& col, vec2 const& uv) { pushPos(pos); pushNorm(norm); pushCol(col); pushUv(uv); }

			struct vert {
				vec3 pos;
				vec3 norm;
				vec4 col;
				vec2 uv;
			};
			void pushVert(vert const& v) { pushPos(v.pos); pushNorm(v.norm); pushCol(v.col); pushUv(v.uv); }
			vert getVert(int i) const { return { pos[i], norm[i], col[i], uv[i] }; }

			vert operator[](int i) const { return getVert(i); }

			geommesh3 operator+(vert const& v) { geommesh3 res = *this; res += v; return res; }
			geommesh3& operator+=(vert const& v) { pushVert(v); return *this; }

			geommesh3 operator+(geommesh3 const& m) const {
				geommesh3 res = *this;
				res += m;
				return res;
			}

			geommesh3& operator+= (geommesh3 const& m) {
				for (int i = 0; i < m.idx.size(); i++) { idx.push_back(pos.size() / 3 + m.idx[i]); }

				pos.insert(pos.end(), m.pos.begin(), m.pos.end());
				norm.insert(norm.end(), m.norm.begin(), m.norm.end());
				col.insert(col.end(), m.col.begin(), m.col.end());
				uv.insert(uv.end(), m.uv.begin(), m.uv.end());

				mat.texpaths.insert(mat.texpaths.begin(), m.mat.texpaths.begin(), m.mat.texpaths.end());

				return *this;
			}

			geommesh3 operator*(mat4 const& m) const {
				geommesh3 rm = *this;
				rm *= m;
				return rm;
			}

			geommesh3& operator*= (mat4 const& m) {
				for (int i = 0; i < pos.size() / 3; i++) {
					int acidx = i * 3;
					vec3 p = m * vec3(pos[acidx], pos[acidx + 1], pos[acidx + 2]);
					pos[acidx] = p.x; pos[acidx + 1] = p.y; pos[acidx + 2] = p.z;
				}
				return *this;
			}

			void reserve(size_t vertNum) {
				pos.reserve(vertNum * 3);
				norm.reserve(vertNum * 3);
				col.reserve(vertNum * 4);
				uv.reserve(vertNum * 2);
				idx.reserve(vertNum);
			}

			void clear() { pos.clear(); norm.clear(); col.clear(); uv.clear(); idx.clear(); mat.texpaths.clear(); }

			int size() const { return pos.size(); }

			size_t getSize() {
				return	(pos.size() + norm.size() + col.size() + uv.size()) * sizeof(float) +
					(idx.size()) * sizeof(unsigned int);
			}
		};

		geommesh3 quad(vec3 pos[4], vec4 col[4], vec2 uv[4]);
		geommesh3 quad(mat4 const& tr, vec4 const& col);

		geommesh3 triangle(vec3 pos[3], vec4 col[3], vec2 uv[3]);
		geommesh3 triangle(mat4 const& tr, vec4 col[3], vec2 uv[3]);

		geommesh3 circle(mat4 const& tr, int iters);

		geommesh3 cube(mat4 const& tr, vec4 const& col = 1.f);
		geommesh3 cubeShared(mat4 const& tr, vec4 const& col = 1.f);
		geommesh3 cylinder(mat4 const& tr, const int iterations = 40);
		geommesh3 sphere(mat4 const& tr);
		geommesh3 torus(mat4 const& tr);
		geommesh3 pyramid(mat4 const& tr);
	}

	namespace geom3 = geom3D;
}
