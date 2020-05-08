#include "ShaderStorageBufferObject.h"

void ShaderStorageBufferObject::bind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
}

void ShaderStorageBufferObject::unbind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBufferObject::update(int shaderSize, void *shaderData) {
	bind();

	GLvoid *p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(p, shaderData, shaderSize);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	unbind();
}

ShaderStorageBufferObject::ShaderStorageBufferObject() {
	
}


ShaderStorageBufferObject::ShaderStorageBufferObject(Shader * s, const char *blockName, int bindingPoint, int shaderSize, void *shaderData) {
	glGenBuffers(1, &id);
	bind();

	glBufferData(GL_SHADER_STORAGE_BUFFER, shaderSize, shaderData, GL_DYNAMIC_COPY); 

	update(shaderSize, shaderData);

	bind();

	blockIndex = glGetProgramResourceIndex(s->program, GL_SHADER_STORAGE_BLOCK, blockName);

	bindingPointIndex = bindingPoint;
	glShaderStorageBlockBinding(s->program, blockIndex, bindingPointIndex);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPointIndex, id);

	unbind();
}


ShaderStorageBufferObject::~ShaderStorageBufferObject(){

}
