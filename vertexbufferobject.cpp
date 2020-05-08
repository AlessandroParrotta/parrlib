#include "VertexBufferObject.h"

#include <vector>
#include <iostream>

void VertexBufferObject::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBufferObject::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::draw() {
	//glDrawArrays(GL_TRIANGLES, 0, data.size());
	glDrawArrays(GL_TRIANGLES, 0, size);
}

void VertexBufferObject::setup(int listPos) {
	bind();
		//glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], drawType);

		glVertexAttribPointer(listPos, dataType, GL_FLOAT, false, 0, NULL);
	unbind();
}

void VertexBufferObject::setupForVAO(int index) {
	bind(); 
		//glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], drawType);

		glVertexAttribPointer(index, dataType, GL_FLOAT, false, 0, NULL);
	unbind();
}

void VertexBufferObject::dispose() {
	if (id == 0) return;

	bind();
		glDeleteBuffers(1, &id);
		id = 0;
		size = 0;
	unbind();
}

void VertexBufferObject::modifyValues(std::vector<float> const& v, int offset) {
	bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset * dataType * sizeof(float), v.size() * sizeof(float), &v[0]);
	unbind();
}
void VertexBufferObject::modifyValues(std::vector<float> const& v) { modifyValues(v, 0); }

void VertexBufferObject::setData(std::vector<float> const& data, GLsizei const& size) {
	this->size = size;
	bind();
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), &data[0], drawType);
	unbind();
}

//void VertexBufferObject::setData(std::vector<vec2> const& data) {
//	std::vector<float> adata;
//	adata.reserve(data.size() * 2);
//	for (int i = 0; i < data.size(); i++) {
//		adata.push_back(data[i].x);
//		adata.push_back(data[i].y);
//	}
//	setData(adata);
//}
//void VertexBufferObject::setData(std::vector<vec3> const& data) {
//	std::vector<float> adata;
//	adata.reserve(data.size() * 3);
//	for (int i = 0; i < data.size(); i++) {
//		adata.push_back(data[i].x);
//		adata.push_back(data[i].y);
//		adata.push_back(data[i].z);
//	}
//	setData(adata);
//}
//void VertexBufferObject::setData(std::vector<vec4> const& data) {
//	std::vector<float> adata;
//	adata.reserve(data.size() * 4); 
//	for (int i = 0; i < data.size(); i++) { 
//		adata.push_back(data[i].x);
//		adata.push_back(data[i].y);
//		adata.push_back(data[i].z);
//		adata.push_back(data[i].w); 
//	}
//	setData(adata);
//}

void VertexBufferObject::setData(std::vector<float> const& data) {
	setData(data, data.size());
}

bool VertexBufferObject::null() { return id == 0; }

VertexBufferObject::VertexBufferObject() {

}

VertexBufferObject::VertexBufferObject(std::vector<float> const& data, int dataType, int drawType) {
	size = data.size();
	this->dataType = dataType;
	this->drawType = drawType;

	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vertPointer = data.size();
}

VertexBufferObject::VertexBufferObject(std::vector<float> const& points, int dataType) : VertexBufferObject(points, dataType, GL_DYNAMIC_DRAW) {}


VertexBufferObject::~VertexBufferObject() {

}
