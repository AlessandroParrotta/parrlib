#include "indexbufferobject.h"

#include "../parrlib/debug.h"

#include "common.h"

namespace gl {
	void IndexBufferObject::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
	void IndexBufferObject::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	void IndexBufferObject::modifyValues(std::vector<unsigned int> const& v, int offset) {
		for (int i = offset; i < data.size(); i++) data[i] = v[i];
		bind();

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, v.size() * sizeof(unsigned int), &v[0]);

		unbind();
	}
	void IndexBufferObject::modifyValues(std::vector<unsigned int> const& v) { modifyValues(v, 0); }

	void IndexBufferObject::setData(std::vector<unsigned int> const& data) {
		size = data.size();
		this->data = data;
		bind();

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_DYNAMIC_DRAW);

		unbind();
	}

	void IndexBufferObject::resize(size_t size) {
		std::vector<unsigned int> t; t.resize(size); setData(t);
	}

	void IndexBufferObject::init() { glGenBuffers(1, &id); }

	IndexBufferObject::IndexBufferObject() {}
	IndexBufferObject::IndexBufferObject(GLuint id) : id(id) {}

	IndexBufferObject::IndexBufferObject(std::vector<unsigned int> const& data) {
		this->data = data;
		init();

		setData(data);
	}

	void IndexBufferObject::draw() {
		bind();

		//deb::msbinfo("DRAW ELEMENTS");
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)0);
		//glDrawElements(GL_TRIANGLES, size/3, GL_UNSIGNED_INT, &data[0]);

		unbind();

		if (com::checkForGLErrors())std::terminate();
	}

	void IndexBufferObject::dispose() {

	}

	extern "C" __declspec(dllexport) impl::IndexBuffer_base * createIndexBuffer_1(std::vector<unsigned int> const& idxs) { return new IndexBufferObject(idxs); }
}