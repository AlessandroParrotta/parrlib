#include "geom3d.h"

#include "../vector3f.h"
#include "../stringutils.h"
#include "../otherutil.h"

//#include "../../debug.h"

namespace prb {
	namespace geom3D {

		geommesh3 triangle(vec3 pos[3], vec4 col[3], vec2 uv[3]) {
			vec3 norm = (pos[1] - pos[0]).cross(pos[2] - pos[0]);
			return {
				{	pos[0], pos[1], pos[2]	},
				{	norm,norm,norm			},
				{	col[0], col[1], col[2]	},
				{	uv[0],uv[1],uv[2]		},
				{	0,2,1					}
			};
		}

		geommesh3 triangle(mat4 const& tr, vec4 col[3], vec2 uv[3]) {
			std::vector<vec3> pos = {
				tr * vec3{ -1.f,	-1.f,	0.f },
				tr * vec3{ -1.f,	1.f,	0.f },
				tr * vec3{	1.f,	1.f,	0.f },
			};

			vec3 n = tr.rotation() * vec3 { 0.f, 0.f, 1.f };
			std::vector<vec3> norm = {
				n,
				n,
				n,
			};

			std::vector<vec4> colr = { { 1.f }, { 1.f }, { 1.f } };
			std::vector<vec2> uvr = { { 0.f,0.f },  { 0.f,1.f },  { 1.f,1.f } };
			std::vector<unsigned int> idx = { 0, 1, 2 };

			return { pos, norm, colr, uvr, idx };
		}

		geommesh3 quad(vec3 pos[4], vec4 col[4], vec2 uv[4]) {
			vec3 norm = (pos[1] - pos[0]).cross(pos[3] - pos[0]);
			return {
				{	pos[0], pos[1], pos[2], pos[3]	},
				{	norm, norm, norm, norm			},
				{	col[0], col[1], col[2], col[3]	},
				{	uv[0], uv[1], uv[2], uv[3]		},
				{	0,1,2, 0,2,3					}
			};
		}

		geommesh3 quad(mat4 const& tr, vec4 const& col) {
			std::vector<vec3> pos = {
				tr * vec3{ -1.f,	-1.f,	0.f },
				tr * vec3{ -1.f,	1.f,	0.f },
				tr * vec3{	1.f,	1.f,	0.f },
				tr * vec3{	1.f,	-1.f,	0.f }
			};

			vec3 n = tr.rotation() * vec3 { 0.f, 0.f, -1.f };
			std::vector<vec3> norm = {
				n,
				n,
				n,
				n
			};

			std::vector<vec4> tcol = { col,col,col,col };
			std::vector<vec2> uv = { { 0.f,0.f },  { 0.f,1.f },  { 1.f,1.f },  { 1.f,0.f } };
			std::vector<unsigned int> idx = { 0, 1, 2,  0, 2, 3 };

			return { pos, norm, tcol, uv, idx };
		}

		geommesh3 circle(mat4 const& tr, int iters) {
			std::vector<vec3> pos, norm; std::vector<vec4> col; std::vector<vec2> uv;
			std::vector<unsigned int> idx;

			pos.reserve(iters); norm.reserve(iters); col.reserve(iters); uv.reserve(iters);
			idx.reserve(iters);

			pos.push_back(tr * vec3(0.f)); norm.push_back(vec3z(-1.f)); col.push_back(1.f);
			for (int i = 0; i < iters; i++) {
				//deb::ss << i << "\n";
				float theta = (float)i / (float)iters * cst::PI*2.f;
				pos.push_back(tr * vec3(sinf(theta), cosf(theta), 0.f));
				norm.push_back(vec3z(-1.f)); col.push_back(1.f);
				uv.push_back(1.f); //TODO: fix uv, and idx
			}

			for (int i = 1; i < iters-1; i++) {
				idx.push_back(i + 1);
				idx.push_back(i);
				idx.push_back(0);
			}
			idx.push_back(0); idx.push_back(iters-1); idx.push_back(1);

			return { pos, norm, col, uv, idx };
		}

		geommesh3 cube(mat4 const& tr, vec4 const& col) {
			geommesh3 res;
			res.reserve(4 * 6); //4 per face since they're not shared

			vec3 pos[]{
				tr * vec3(-1.f,	-1.f,	1.f),
				tr * vec3(-1.f,	1.f,	1.f),
				tr * vec3(1.f,	1.f,	1.f),
				tr * vec3(1.f,	-1.f,	1.f),

				tr * vec3(-1.f,	-1.f,	-1.f),
				tr * vec3(-1.f,	1.f,	-1.f),
				tr * vec3(1.f,	1.f,	-1.f),
				tr * vec3(1.f,	-1.f,	-1.f),
			};

			vec4 colt[]{ col,col,col,col };
			vec2 uvt[]{
				vec2(0.f,0.f),
				vec2(0.f,1.f),
				vec2(1.f,1.f),
				vec2(1.f,0.f)
			};

			vec3 tpos[4]{ 0.f,0.f,0.f,0.f };
			tpos[0] = pos[0]; tpos[1] = pos[1]; tpos[2] = pos[2]; tpos[3] = pos[3]; res += quad(tpos, colt, uvt);
			tpos[0] = pos[3]; tpos[1] = pos[2]; tpos[2] = pos[6]; tpos[3] = pos[7]; res += quad(tpos, colt, uvt);
			tpos[0] = pos[7]; tpos[1] = pos[6]; tpos[2] = pos[5]; tpos[3] = pos[4]; res += quad(tpos, colt, uvt);
			tpos[0] = pos[4]; tpos[1] = pos[5]; tpos[2] = pos[1]; tpos[3] = pos[0]; res += quad(tpos, colt, uvt);

			tpos[0] = pos[1]; tpos[1] = pos[5]; tpos[2] = pos[6]; tpos[3] = pos[2]; res += quad(tpos, colt, uvt);
			tpos[0] = pos[3]; tpos[1] = pos[7]; tpos[2] = pos[4]; tpos[3] = pos[0]; res += quad(tpos, colt, uvt);

			return res;
		}

		geommesh3 cubeShared(mat4 const& tr, vec4 const& col) {
			std::vector<vec3> pos = {
				tr * vec3{ -1.f,	-1.f,	-1.f },
				tr * vec3{ -1.f,	1.f,	-1.f },
				tr * vec3{	1.f,	1.f,	-1.f },
				tr * vec3{	1.f,	-1.f,	-1.f },

				tr * vec3{	-1.f,	-1.f,	1.f },
				tr * vec3{	-1.f,	1.f,	1.f },
				tr * vec3{	1.f,	1.f,	1.f },
				tr * vec3{	1.f,	-1.f,	1.f }
			};

			vec3 normt = tr.rotation() * vec3 { 0.f, 0.f, 1.f };
			std::vector<vec3> norm = {
				normt,
				normt,
				normt,
				normt,

				normt,
				normt,
				normt,
				normt
			};

			std::vector<vec4> colt = { col,col,col,col, col,col,col,col };
			std::vector<vec2> uv = { { 0.f,0.f },  { 0.f,1.f },  { 1.f,1.f },  { 1.f,0.f },{ 0.f,0.f },  { 0.f,1.f },  { 1.f,1.f },  { 1.f,0.f } };
			std::vector<unsigned int> idx = {
				0, 2, 1, //front
				0, 3, 2,

				3, 6, 2, //right
				3, 7, 6,

				7, 4, 5, //back
				7, 5, 6,

				0, 1, 4, //left
				4, 1, 5,

				0, 4, 7, //down
				0, 7, 3,

				1, 6, 5, //up
				1, 2, 6,
			};

			return { pos, norm, colt, uv, idx };
		}

		geommesh3 cylinder(mat4 const& tr, const int iterations) {
			geommesh3 res = 
				circle(pmat4::translate({ 0.f,1.f,0.f }) * pmat4::rotate(90.f, v3ax::riv3), iterations) +
				circle(pmat4::translate({ 0.f,-1.f,0.f }) * pmat4::rotate(90.f, v3ax::riv3), iterations);

			res.reserve(res.size() * 2);

			int firstStartIdx = res.size();
			for (int i = 0; i < iterations; i++) {
				res += res[1+i];
			}

			int secondStartIdx = res.size();
			for (int i = 0; i < iterations; i++) {
				res += res[1+iterations + 1+i];
			}

			int curIdx = res.size();
			for (int i = 1; i < iterations-1; i++) {
				int otherIdx = curIdx + 1+iterations + 1+i;

				res.idx.push_back(firstStartIdx + i);
				res.idx.push_back(secondStartIdx + i+1);
				res.idx.push_back(firstStartIdx + i+1);

				res.idx.push_back(firstStartIdx + i);
				res.idx.push_back(secondStartIdx + i);
				res.idx.push_back(secondStartIdx + i +1);

				//deb::ss << (firstStartIdx + i) << " " << (secondStartIdx + i + 1) << " " << (firstStartIdx + i + 1) << "\n";
				//deb::ss << (firstStartIdx + i) << " " << (secondStartIdx + i) << " " << (secondStartIdx + i + 1) << "\n";
			}

			return res;
		}

		vec4 randCol() { return { outl::frand(), outl::frand(), outl::frand(), 1.f }; }

		void addNodeMeshes(const char* fileName, aiMaterial** mats, aiMesh** meshList, aiNode* n, geom3::geommesh3& gm) {
			if (!meshList || !n) return;

			aiMatrix4x4 aitr = n->mTransformation;
			mat4 tr;
			for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) tr[y][x] = aitr[x][y];

			for (int i = 0; i < n->mNumMeshes; i++) {
				aiMesh* m = meshList[n->mMeshes[i]];

				geom3::geommesh3 tgm;
				tgm.reserve(m->mNumVertices);

				for (int i = 0; i < mats[m->mMaterialIndex]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE); i++) {
					aiString tpath;
					mats[m->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &tpath, 0, 0, 0, 0, 0);
					tgm.mat.texpaths.push_back(stru::splitFilePath(fileName)[0] + tpath.C_Str());
					//std::cout << "node contains diffuse texture " << (stru::splitFilePath(fileName)[0] + tpath.C_Str()) << "\n";
				}

				for (int i = 0; i < mats[m->mMaterialIndex]->GetTextureCount(aiTextureType::aiTextureType_NORMALS); i++) {
					aiString tpath;
					mats[m->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &tpath, 0, 0, 0, 0, 0);
					tgm.mat.texpaths.push_back(stru::splitFilePath(fileName)[0] + tpath.C_Str());
					//std::cout << "node contains normal texture " << (stru::splitFilePath(fileName)[0] + tpath.C_Str()) << "\n";
				}

				for (int i = 0; i < m->mNumVertices; i++) {
					aiVector3D vert = m->mVertices[i];
					aiVector3D norm = m->mNormals[i];
					aiVector3D uv = m->mTextureCoords[0][i];

					tgm.pushPos(tr * vec3(vert.x, vert.y, vert.z));
					tgm.pushCol(1.f); //tgm.pushCol(randCol());
					tgm.pushNorm(tr.rotation() * -vec3(norm.x, norm.y, norm.z));
					tgm.pushUv(vec2(uv.x, uv.y));
				}

				for (int i = 0; i < m->mNumFaces; i++) {
					aiFace f = m->mFaces[i];

					tgm.pushIdx(f.mIndices[2]);
					tgm.pushIdx(f.mIndices[1]);
					tgm.pushIdx(f.mIndices[0]);
				}

				gm += tgm;
			}

			for (int i = 0; i < n->mNumChildren; i++) {
				addNodeMeshes(fileName, mats, meshList, n->mChildren[i], gm);
			}
		}

		geommesh3 fromFileSingle(const char* fileName) {
			geommesh3 rm;

			//Timer t;

			Assimp::Importer imp;
			const aiScene* sc = imp.ReadFile(stru::fallbackPath(outl::getExeFolder(), fileName), aiProcess_Triangulate);
			if (!sc) std::cout << "could not load scene!\n";
			else {
				addNodeMeshes(fileName, sc->mMaterials, sc->mMeshes, sc->mRootNode, rm);
			}

			//t.set(); deb::tss << "assimp geom3 time " << t.getTime().c_str() << "\n"; deb::outStr();

			return rm;
		}
	}
}
