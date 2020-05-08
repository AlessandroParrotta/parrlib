#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include <math.h>

#include "vertexbufferobject.h"
#include "indexbufferobject.h"

class VertexArrayObject{
public:
	GLuint id = 0;

	std::vector<VBO> vbos;
	std::vector<IBO> ibos;

	void bind();
	void unbind();

	//void addSingleVBO(VertexBufferObject vbo);
	//void addVBO(VertexBufferObject vbo, VertexBufferObject color, VertexBufferObject texCoords);
	//void addVBO(VertexBufferObject vbo, VertexBufferObject normal, VertexBufferObject color, VertexBufferObject texCoords);
	//void addVBO(VertexBufferObject vbo, VertexBufferObject color);

	void addVBO(VertexBufferObject vbo);
	void setVbos(std::vector<VertexBufferObject> vbos);

	void addIBO(IBO const& ibo);
	void setIbos(std::vector<IBO> const& ibos);

	void clearVbos();
	void clearIbos();

	void clearBuffers();

	//void draw(int list, int imageSizeX, int imageSizeY, bool useColor);
	//void draw(int list, bool useColor);

	void draw();
	void draw(Shader *sh);

	void dispose();

	void init();

	bool null();

	VertexArrayObject();
	VertexArrayObject(std::vector<VertexBufferObject> vbos);
	VertexArrayObject(std::vector<VertexBufferObject> vbos, std::vector<IBO> const& ibos);
	//VertexArrayObject(std::vector<int> vbosizes);
	//VertexArrayObject(VertexBufferObject vbo, VertexBufferObject normal, VertexBufferObject color, VertexBufferObject texCoords);
	~VertexArrayObject();
};

typedef VertexArrayObject VAO;
