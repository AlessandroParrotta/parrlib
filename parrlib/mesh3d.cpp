#include "mesh3d.h"
#include "globals.h"

#include "math/stringutils.h"
#include "debug.h"

namespace prb {
	Mesh3D::Mesh3D() {}

	Mesh3D::Mesh3D(geom3::geommesh3 const& m) {
		Timer t;

		ib = { m.idx };
		vb = { { (interleaveVB(interleaveVB(interleaveVB(m.pos, 3, m.norm, 3), 3+3, m.col, 4), 3+3+4, m.uv, 2)) },  { 3,3,4,2 } };
		if (m.mat.texpaths.size() > 0) { tex = stru::fallbackPath(outl::getExeFolder(), m.mat.texpaths[0]).c_str(); }
		
		t.set(); deb::tss << "mesh3 geom3 time " << t.getTime().c_str() << "\n"; deb::outStr();
	}
	Mesh3D::Mesh3D(const char* fileName) : Mesh3D(geom3::geommesh3(fileName)) {}

	void Mesh3D::draw() {
		vb.use();

		ib.draw();

		vb.release();
	}

	void Mesh3D::drawDefaultShader(mat4 const& proj, mat4 const& tr) {
		Shader& sh = glb::getShader("shaders/default3D", { 
			{ "POSITION",	"r32g32b32_float" }, 
			{ "NORMAL",		"r32g32b32_float" }, 
			{ "COLOR",		"r32g32b32a32_float" }, 
			{ "TEXCOORD",	"r32g32_float" },
		});

		sh.use();
			sh.setUniform("mat", proj * tr);
			sh.setUniform("textured", (int)!tex.null());
			if (!tex.null()) {
				sh.setUniform("tex", 0);
				tex.bind();
			}

			//sh.setUniform("rotm", tr.transposed().inverted().rotation());
			//sh.setUniform("dirlight", vec3(1.f));
			//sh.setUniform("dirlight", vec3(1.f));

			draw();

		sh.release();
	}

	bool Mesh3D::null() { return !ib.ptr || !vb.ptr; }
}
