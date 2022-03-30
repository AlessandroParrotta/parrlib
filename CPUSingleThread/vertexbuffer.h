#pragma once

#include <windows.h>

#include "../parrlib/impl/vertexbuffer.h"

namespace softst {
	using namespace prb;
	class VertexBuffer : public impl::VertexBuffer_base {
	public:
		std::vector<float> data;
		std::vector<unsigned int> dataTypes;

		VertexBuffer();
		VertexBuffer(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset);
		VertexBuffer(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset);
		VertexBuffer(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset);

		void resize(size_t size); //in bytes
		void setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes);

		void use();
		void release();

		void draw();

		void dispose();
	};

	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_1();
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_2(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset);
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_3(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset);
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_4(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset);

	typedef VertexBuffer vbuf;
}
