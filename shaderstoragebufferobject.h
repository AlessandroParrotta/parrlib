#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Shader.h"

class ShaderStorageBufferObject{
public:
	GLuint id;
	GLuint blockIndex;
	GLuint bindingPointIndex;

	int shaderSize;
	void *shaderData;

	void update(int shaderSize, void *shaderData);

	void bind();
	void unbind();

	ShaderStorageBufferObject();
	ShaderStorageBufferObject(Shader * s, const char *blockName, int bindingPoint, int shaderSize, void *shaderData);

	~ShaderStorageBufferObject();
};

