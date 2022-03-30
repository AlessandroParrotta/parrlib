#pragma once

#include <vector>

#include "impl/impl.h"

namespace prb {
	class VertexBuffer {
	public:
		impl::VertexBuffer_base* ptr = NULL;
		std::vector<unsigned int> dataTypes;

		VertexBuffer();
		VertexBuffer(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, unsigned int offset);
		VertexBuffer(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, unsigned int  offset);
		VertexBuffer(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes);
		VertexBuffer(size_t byteSize, std::vector<unsigned int> const& dataTypes, unsigned int offset);
		VertexBuffer(std::vector<unsigned int> const& dataTypes);

		void resize(size_t size); //in bytes
		void setData(std::vector<float> const& data);
		void setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes);

		void use();
		void release();

		void draw();

		void dispose();
	};

	typedef VertexBuffer vbuf;

}
