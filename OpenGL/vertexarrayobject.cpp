#include "VertexArrayObject.h"

#include "common.h"

namespace gl {
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

		if (com::checkForGLErrors()) std::terminate();

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

		if (com::checkForGLErrors()) std::terminate();
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

		if (com::checkForGLErrors()) std::terminate();

		for (VBO v : vbos) v.dispose();
		vbos.clear();

		if (com::checkForGLErrors()) std::terminate();
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

		if (com::checkForGLErrors()) std::terminate();

		bind();

		if (com::checkForGLErrors()) std::terminate();

		//for (int i = 0; i < vbos.size(); i++) glEnableVertexAttribArray(i);

		//deb::debss << "DRAW VAO " << vbos.size() << " " << ibos.size() << "\n"; deb::outStr();
		//for (int i = 0; i < vbos.size(); i++) { deb::debss << vbos[i].size << " "; } deb::debss << "\n";  deb::outStr();
		//for (int i = 0; i < ibos.size(); i++) { deb::debss << ibos[i].size << " "; } deb::debss << "\n";  deb::outStr();

		if (ibos.size() == 0) vbos[0].draw();
		else {
			ibos.back().draw();
		}

		if (com::checkForGLErrors()) std::terminate();

		//for (int i = 0; i < vbos.size(); i++) glDisableVertexAttribArray(i);

		unbind();
	}

	void VertexArrayObject::draw(Shader* sh) {
		if (!sh) return;

		sh->use();

		draw();

		sh->release();

		if (com::checkForGLErrors()) std::terminate();
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
		if (com::checkForGLErrors()) std::terminate();

		if (id != 0) { dispose(); id = 0; };
		glGenVertexArrays(1, &id);

		if (com::checkForGLErrors()) std::terminate();
	}

	VertexArrayObject::VertexArrayObject() {

	}

	VertexArrayObject::VertexArrayObject(std::vector<VertexBufferObject> vbos) {
		if (id == 0) init();

		setVbos(vbos);
	}

	VertexArrayObject::VertexArrayObject(std::vector<VertexBufferObject> vbos, std::vector<IBO> const& ibos) {
		if (id == 0) init();

		setVbos(vbos);
		setIbos(ibos);
	}

	VertexArrayObject::VertexArrayObject(std::vector<float> const& verts, std::vector<unsigned int> const& dataTypes) {
		if (id == 0) init();

		if (com::checkForGLErrors()) std::terminate();

		//separate verts in multiple vertex buffer objects based on their datatype (vec2, vec3, vec4, etc..)

		//int dataSum = 0; for (int i = 0; i < dataTypes.size(); i++) dataSum += dataTypes[i];

		//std::vector<std::vector<float>> data; data.resize(dataTypes.size());
		//for (int i = 0; i < verts.size(); i += dataSum) {
		//	if (i + dataSum > verts.size()) break;

		//	int curOff = 0;											//current offset
		//	for (int j = 0; j < dataTypes.size(); j++) {
		//		for (int n = 0; n < dataTypes[j]; n++) {
		//			data[j].push_back(verts[i + curOff + n]);
		//		}
		//		curOff += dataTypes[j];
		//	}
		//}

		//for (int i = 0; i < dataTypes.size(); i++) {
		//	data[i].reserve(verts.size() / dataSum * dataTypes[i]);

		//	size_t initSkip = 0; for (int j = 0; j < i; j++) initSkip += dataTypes[j];
		//	size_t skipValue = 0; for (int j = 0; j < dataTypes.size(); j++) if (i != j) skipValue += dataTypes[j];

		//	if (skipValue == 0) continue;

		//	for (int j = initSkip; j < verts.size(); j += dataSum) {
		//		for (int n = 0; n < dataTypes[i]; n++) {
		//			//if (i > 0) { deb::debss << initSkip << " " << dataSum << " " << i << " -- " << j << " + " << n << " = " << (j + n); deb::msbinfo(); }
		//			data[i].push_back(verts[j + n]);
		//		}
		//	}
		//}


		std::vector<VertexBufferObject> vbos; vbos.resize(dataTypes.size());
		//for (int i = 0; i < vbos.size(); i++) vbos[i] = VBO(data[i], dataTypes[i]);
		
		if (com::checkForGLErrors()) std::terminate();

		for (int i = 0; i < vbos.size(); i++) vbos[i] = VBO({ 0.f }, dataTypes[i]); 
		if (com::checkForGLErrors()) std::terminate();
		setVbos(vbos);

		if (com::checkForGLErrors()) std::terminate();

		int dataSum = 0; for (int i = 0; i < dataTypes.size(); i++) dataSum += dataTypes[i];

		//std::vector<unsigned int> idxs; idxs.reserve(verts.size() / dataSum);
		//for (int i = 0; i < verts.size() / dataSum; i++) idxs.push_back(i);
		//IndexBufferObject ibo = IBO(idxs);
		//ibos.push_back(ibo);

		setData(verts, dataTypes);

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

	VertexArrayObject::~VertexArrayObject() {
		//clearBuffers();
	}

	void VertexArrayObject::resize(size_t size) {
		for (int i = 0; i < vbos.size(); i++) vbos[i].resize(size);
		for (int i = 0; i < ibos.size(); i++) ibos[i].resize(size);
	}

	void VertexArrayObject::setData(std::vector<float> const& verts, std::vector<unsigned int> const& dataTypes) {
		if (dataTypes.size() != vbos.size()) {
			//for (int i = 0; i < vbos.size(); i++) vbos[i].dispose();
			//vbos.clear();

			//vbos.resize(dataTypes.size());
			//for (int i = 0; i < vbos.size(); i++) vbos[i] = VBO({}, dataTypes[i]);
			deb::tss << "OpenGL BACKEND Warning: VAO dataTypes number passed to setData method (" << dataTypes.size() << ") is different from the original dataTypes this VAO has been constructed with (" << vbos.size() << ")\n"; deb::outStr();
		}

		//if (com::checkForGLErrors()) std::terminate();

		int dataSum = 0; for (int i = 0; i < dataTypes.size(); i++) dataSum += dataTypes[i];

		//deb::outStr("INSERT\n");

		std::vector<std::vector<float>> data; data.resize(dataTypes.size());
		for (int i = 0; i < dataTypes.size(); i++) {
			data[i].reserve(verts.size() / dataSum * dataTypes[i]);

			size_t initSkip = 0; for (int j = 0; j < i; j++) initSkip += dataTypes[j];
			size_t skipValue = 0; for (int j = 0; j < dataTypes.size(); j++) if (i != j) skipValue += dataTypes[j];

			if (skipValue == 0) continue;

			for (int j = initSkip; j < verts.size(); j += dataSum) {
				for (int n = 0; n < dataTypes[i]; n++) {
					//if (i > 0) { deb::debss << initSkip << " " << dataSum << " " << i << " -- " << j << " + " << n << " = " << (j + n); deb::msbinfo(); }
					//deb::debss << "{ "; for (int k = 0; k < dataTypes.size(); k++) deb::debss << dataTypes[k] << (k == dataTypes.size() - 1 ? "" : ", "); deb::debss << " } "; deb::debss << initSkip << " " << dataSum << " " << i << " -- " << j << " + " << n << " = " << (j + n) << "/" << verts.size() << "\n"; deb::outStr();
					data[i].push_back(verts[j+n]);
				}
			}
		}

		//deb::outStr("VERTS STRUCTURE\n");

		//for (int i = 0; i < verts.size(); i += dataSum) {
		//	for (int j = 0; j < dataSum; j++) deb::debss << verts[i+j] << " ";
		//	deb::debss << "\n";
		//}
		//deb::outStr();

		//deb::outStr("DATA STRUCTURES\n");

		//for (int i = 0; i < dataTypes.size(); i++) {
		//	deb::debss << "DATA STRUCTURE " << dataTypes[i] << "\n"; deb::outStr();

		//	for (int j = 0; j < data[i].size(); j+=dataTypes[i]) {
		//		for (int k = 0; k < dataTypes[i]; k++) deb::debss << data[i][j + k] << " ";
		//		deb::debss << "\n";
		//	}
		//	deb::outStr();
		//}

		//if (com::checkForGLErrors()) std::terminate();

		for (int i = 0; i < vbos.size(); i++) vbos[i].setData(data[i]); 

		if (com::checkForGLErrors()) std::terminate();

		//std::vector<unsigned int> idxs; idxs.reserve(verts.size() / dataSum);
		//for (int i = 0; i < verts.size() / dataSum; i++) idxs.push_back(i);
		//if (ibos.size() == 0) { ibos.push_back({ idxs }); }
		//else { ibos.back().setData(idxs); }

		//deb::outStr("INDEX STRUCTURE\n");
		//for (int i = 0; i < idxs.size(); i += 3) {
		//	if (i + 3 > idxs.size()) break;
		//	deb::debss << idxs[i+0] << " " << idxs[i+1] << " " << idxs[i+2] << "\n"; deb::outStr();
		//}
	}

	void VertexArrayObject::use() {
		bind();
	}

	void VertexArrayObject::release() {
		unbind();
	}

	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_1() { return new VertexArrayObject(); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_2(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) { 
		size_t floatNum = std::floor(byteSize / sizeof(float));
		std::vector<float> tdata; tdata.resize(floatNum);
		return new VertexArrayObject(tdata, dataTypes);  //TODO broken, doesn't use data
	}
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_3(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset) { return new VertexArrayObject(data, dataTypes); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_4(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) {
		size_t floatNum = std::floor(byteSize / sizeof(float));
		//deb::debss << "Float num: " << floatNum << "\n";  deb::outStr();
		std::vector<float> data; data.resize(floatNum);
		return new VertexArrayObject(data, dataTypes);
	}

}