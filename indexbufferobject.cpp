#include "indexbufferobject.h"

void IndexBufferObject::bind()		{ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
void IndexBufferObject::unbind()	{ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	}

void IndexBufferObject::modifyValues(std::vector<unsigned int> const& v, int offset){
	bind();

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, v.size() * sizeof(unsigned int), &v[0]);

	unbind();
}
void IndexBufferObject::modifyValues(std::vector<unsigned int> const& v){ modifyValues(v, 0); }

void IndexBufferObject::setData(std::vector<unsigned int> const& data) {
	size = data.size();
	bind();

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_DYNAMIC_DRAW);

	unbind();
}

void IndexBufferObject::init() { glGenBuffers(1, &id); }

IndexBufferObject::IndexBufferObject(){}
IndexBufferObject::IndexBufferObject(GLuint id) : id(id){}

IndexBufferObject::IndexBufferObject(std::vector<unsigned int> const& data) {
	init();

	setData(data);
}

void IndexBufferObject::draw() {
	bind();

		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)0);

	unbind();
}

void IndexBufferObject::dispose() {

}
