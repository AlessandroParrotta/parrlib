#pragma once

#include <vector>

#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include "../parrlib/impl/indexbuffer.h"

namespace gl {
	using namespace prb;
	class IndexBufferObject : public impl::IndexBuffer_base {
	public:
		GLuint id = 0;

		unsigned int size = 0; //in number of ints
		std::vector<unsigned int> data;

		void bind();
		void unbind();

		void init();

		void modifyValues(std::vector<unsigned int> const& v, int offset);
		void modifyValues(std::vector<unsigned int> const& v);

		void setData(std::vector<unsigned int> const& data);

		void resize(size_t size);

		IndexBufferObject();
		IndexBufferObject(GLuint id);
		IndexBufferObject(std::vector<unsigned int> const& data);

		void draw();

		void dispose();
	};

	extern "C" __declspec(dllexport) impl::IndexBuffer_base * createIndexBuffer_1(std::vector<unsigned int> const& idxs);

	typedef IndexBufferObject IBO;
}
