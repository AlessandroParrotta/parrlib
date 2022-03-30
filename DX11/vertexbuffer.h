#pragma once

#include <vector>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "../parrlib/impl/vertexbuffer.h"

namespace dx11 {
	using namespace prb;

	class VertexBuffer : public impl::VertexBuffer_base {
	public:
		ID3D11Buffer* vptr = NULL;
		UINT stride = 0, offset = 0;
		UINT dataCapacity = 0; //bytes
		UINT vCount = 0; //vertex number

		void init(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subRes);
		void init(const void* data, size_t byteSize, UINT stride, UINT offset);
		void init(std::vector<float> const& data, UINT stride, UINT offset);
		void init(size_t byteSize, UINT stride, UINT offset);

		VertexBuffer();
		VertexBuffer(D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA& subRes);
		VertexBuffer(void* data, size_t byteSize, UINT stride, UINT offset);
		VertexBuffer(std::vector<float> const& data, UINT stride, UINT offset);
		VertexBuffer(size_t byteSize, UINT stride, UINT offset);

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

#ifndef PARRLIBDX_NAMESPACE_SHORTEN
	typedef VertexBuffer vbuf;
#endif

}
