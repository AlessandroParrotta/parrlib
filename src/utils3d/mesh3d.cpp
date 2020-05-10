#include <parrlib/utils3d/mesh3d.h>

Mesh3D::Mesh3D() {}

Mesh3D::Mesh3D(geom3::geommesh3 const& m) {
	vao = { { { m.pos, 3 }, { m.norm, 3 }, { m.col, 4 }, { m.uv, 2 } }, { { m.idx } } };
	if (m.mat.texpaths.size() > 0) tex = m.mat.texpaths[0];
}
Mesh3D::Mesh3D(VAO const& vao) { this->vao = vao; }
Mesh3D::Mesh3D(const char* fileName) : Mesh3D(geom3::geommesh3(fileName)) {}

void Mesh3D::draw() {
	vao.draw();
}

void Mesh3D::drawDefaultShader(mat4 const& proj, mat4 const& tr) {
	Shader& sh = glb::getShader("assets/shaders/mesh3draw");

	sh.use();
	sh.setUniform("textured", !tex.null());
	if (!tex.null()) { sh.setUniform("tex", 0); util::bindTexture(0, tex.texID); }

	sh.setUniform("transform", proj * tr);
	sh.setUniform("rotm", tr.transposed().inverted().rotation());
	sh.setUniform("dirlight", vec3(1.f));
	//sh.setUniform("dirlight", vec3(1.f));

	draw();

	sh.release();
}

bool Mesh3D::null() { return vao.null(); }
