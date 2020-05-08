#pragma once

#include "geom3d.h"

#include <parrlib/vertexarrayobject.h>
#include <parrlib/globals.h>


class Mesh3D{
public:
	VAO vao;
	Texture tex;

	Mesh3D();
	Mesh3D(geom3D::geommesh3 const& m);
	Mesh3D(VAO const& vao);
	Mesh3D(const char* fileName);

	void draw(mat4 const& proj, mat4 const& tr); //draws the mesh (a shader must be used previous to this call)
	void drawDefaultShader(mat4 const& proj, mat4 const& tr); //draws with a default set shader (will override any shader previously used)

	bool null();
};

typedef Mesh3D mesh3;

