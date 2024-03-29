#pragma once

#include <parrlibcore/utils3d/geom3d.h>

#include "indexbuffer.h"
#include "globals.h"

namespace prb {
	class Mesh3D {
	public:
		ibuf ib;
		vbuf vb;
		Texture tex;

		Mesh3D();
		Mesh3D(geom3D::geommesh3 const& m);
		Mesh3D(const char* fileName);

		void draw(); //draws the mesh (a shader must be used previous to this call)
		void drawDefaultShader(mat4 const& proj, mat4 const& tr); //draws with a default set shader (will override any shader previously used)

		bool null();
	};

	typedef Mesh3D mesh3;

}
