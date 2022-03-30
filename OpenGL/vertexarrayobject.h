#pragma once

#include <glad\glad.h>
//#include <GLFW\glfw3.h>
#include "Shader.h"
#include <math.h>

#include "vertexbufferobject.h"
#include "indexbufferobject.h"

#include "../parrlib/impl/vertexbuffer.h"

namespace gl {
	class VertexArrayObject : public impl::VertexBuffer_base {
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
		void draw(Shader* sh);

		void dispose();

		void init();

		bool null();

		VertexArrayObject();
		VertexArrayObject(std::vector<VertexBufferObject> vbos);
		VertexArrayObject(std::vector<VertexBufferObject> vbos, std::vector<IBO> const& ibos);
		//VertexArrayObject(std::vector<int> vbosizes);
		//VertexArrayObject(VertexBufferObject vbo, VertexBufferObject normal, VertexBufferObject color, VertexBufferObject texCoords);
		~VertexArrayObject();

		//IMPL FUNCS
		VertexArrayObject(std::vector<float> const& verts, std::vector<unsigned int> const& dataTypes);
		void resize(size_t size);
		void setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes);
		void use();
		void release();
	};

	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_1();
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_2(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset);
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_3(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset);
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_4(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset);
	typedef VertexArrayObject VAO;
}
