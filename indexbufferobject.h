#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBufferObject{
public:
	GLuint id = 0;

	unsigned int size = 0; //in number of ints

	void bind();
	void unbind();

	void init();

	void modifyValues(std::vector<unsigned int> const& v, int offset);
	void modifyValues(std::vector<unsigned int> const& v);

	void setData(std::vector<unsigned int> const& data);

	IndexBufferObject();
	IndexBufferObject(GLuint id);
	IndexBufferObject(std::vector<unsigned int> const& data);

	void draw();

	void dispose();
};

typedef IndexBufferObject IBO;
