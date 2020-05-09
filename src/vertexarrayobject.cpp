#include <parrlib/VertexArrayObject.h>

void VertexArrayObject::bind() {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	glBindVertexArray(id);
}

void VertexArrayObject::unbind() {
	glBindVertexArray(0);
}

//void VertexArrayObject::addVBO(VertexBufferObject vbo, VertexBufferObject color, VertexBufferObject texCoords) {
//	addSingleVBO(vbo);
//
//	addSingleVBO(color);
//
//	addSingleVBO(texCoords);
//}
//
//void VertexArrayObject::addVBO(VertexBufferObject vbo, VertexBufferObject normal, VertexBufferObject color, VertexBufferObject texCoords) {
//	addSingleVBO(vbo);
//	
//	addSingleVBO(normal);
//
//	addSingleVBO(color);
//
//	addSingleVBO(texCoords);
//}
//
//void VertexArrayObject::addVBO(VertexBufferObject vbo, VertexBufferObject color) {
//	addSingleVBO(vbo);
//
//	addSingleVBO(color);
//
//	float *texCoords = new float[vbo.size];
//	
//	VertexBufferObject texVBO = VertexBufferObject(texCoords, vbo.size, 2, GL_DYNAMIC_DRAW);
//
//	addSingleVBO(texVBO);
//}

//void VertexArrayObject::addSingleVBO(VertexBufferObject vbo) {
//	bind();
//	std::cout << "adding vbo at capacity " << buffers.size() << "\n";
//	vbo.setup(buffers.size());
//
//	unbind();
//
//	buffers.push_back(vbo);
//
//	for (int i = 0; i < buffers.size(); i++) {
//		std::cout << "buffer <" << i << "> size: " << buffers[i].size << "\n";
//	}
//}

void VertexArrayObject::addVBO(VertexBufferObject vbo) {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	bind();
	//std::cout << "bufsize " << buffers.size() << "\n";
	vbo.setupForVAO(vbos.size());

	vbos.push_back(vbo);

	for (int i = 0; i < vbos.size(); i++) glEnableVertexAttribArray(i);
	unbind();

	//for (int i = 0; i < buffers.size(); i++) {
	//	std::cout << "buffer <" << i << "> size: " << buffers[i].size << "\n";
	//}
}

void VertexArrayObject::setVbos(std::vector<VertexBufferObject> vbos) {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	clearVbos();

	bind();
	for (VBO v : vbos) addVBO(v);
	unbind();
}

void VertexArrayObject::addIBO(IBO const& ibo) {
	ibos.push_back(ibo);
}

void VertexArrayObject::setIbos(std::vector<IBO> const& ibos) {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	clearIbos();

	bind();
	for (IBO ib : ibos) addIBO(ib);
	unbind();
}

void VertexArrayObject::clearVbos() {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	for (VBO v : vbos) v.dispose();
	vbos.clear();
}

void VertexArrayObject::clearIbos() {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	for (IBO ib : ibos) ib.dispose();
	ibos.clear();
}

void VertexArrayObject::clearBuffers() {
	clearVbos();
	clearIbos();
}

int getClosestPow(int n) {
	int p = 0;
	int i = 0;
	while (p < n) {
		p = (int)pow(2, i);
		i++;
	}

	return p;
}

//void VertexArrayObject::draw(int list, int imageSizeX, int imageSizeY, bool useColor) {
//	list /= 3;
//	//list /= 2;
//
//	sh.use();
//
//	if (imageSizeX > 0 && imageSizeY > 0) {
//		sh.setUniform1i("texturePassed", 1);
//		sh.setUniform2f("imageSize", imageSizeX, imageSizeY);
//		sh.setUniform2f("imageClosestPow", getClosestPow(imageSizeX), getClosestPow(imageSizeY));
//	}
//
//	sh.setUniform1i("useColor", useColor);
//
//	bind();
//	glEnableVertexAttribArray(list);
//	glEnableVertexAttribArray(list+1);
//	glEnableVertexAttribArray(list+2);
//
//	buffers[list].draw();
//
//	glDisableVertexAttribArray(list);
//	glDisableVertexAttribArray(list + 1);
//	glDisableVertexAttribArray(list + 2);
//	unbind();
//
//	sh.release();
//}

//void VertexArrayObject::draw(int list, bool useColor) {
//	draw(list, 0, 0, useColor);
//}

void VertexArrayObject::draw() {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }
	if (vbos.size() <= 0 /*|| sh == nullptr*/) return;

		bind();

			//for (int i = 0; i < buffers.size(); i++) glEnableVertexAttribArray(i);

				if(ibos.size()==0) vbos[0].draw();
				else {
					ibos.back().draw();
				}

			//for (int i = 0; i < buffers.size(); i++) glDisableVertexAttribArray(i);

		unbind();
}

void VertexArrayObject::draw(Shader *sh) {
	if (!sh) return;

	sh->use();

		draw();

	sh->release();
}

void VertexArrayObject::dispose() {
	if (id == 0) { std::cout << "vao not initialized\n"; return; }

	clearVbos();

	bind();
	glDeleteVertexArrays(1, &id);
	unbind();
}

bool VertexArrayObject::null() { return id == 0; }

void VertexArrayObject::init() {
	if (id != 0) { dispose(); id = 0; };
	glGenVertexArrays(1, &id);
}

VertexArrayObject::VertexArrayObject(){

}

VertexArrayObject::VertexArrayObject(std::vector<VertexBufferObject> vbos){
	if (id == 0) init();

	setVbos(vbos);
}

VertexArrayObject::VertexArrayObject(std::vector<VertexBufferObject> vbos, std::vector<IBO> const& ibos) {
	if (id == 0) init();

	setVbos(vbos);
	setIbos(ibos);
}

//VertexArrayObject::VertexArrayObject(std::vector<int> vbosizes) {
//	if (id == 0) init();
//
//	std::vector<VertexBufferObject> vbos;
//	for (auto& i : vbosizes) vbos.push_back({ {}, i });
//
//	setVbos(vbos);
//}


//VertexArrayObject::VertexArrayObject(VertexBufferObject vbo, VertexBufferObject normal, VertexBufferObject color, VertexBufferObject texCoords){
//	addVBO(vbo, normal, color, texCoords);
//}

VertexArrayObject::~VertexArrayObject(){
	//clearBuffers();
}
