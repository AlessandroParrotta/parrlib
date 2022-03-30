#pragma once

#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>

#include <vector>

#include "../parrlib/impl/indexbuffer.h"

#include "common.h"

namespace dx11 {
	using namespace prb;
	class IndexBuffer : public impl::IndexBuffer_base {
	public:
		ID3D11Buffer* buf = NULL;
		std::vector<unsigned int> data;

		IndexBuffer(std::vector<unsigned int> const& idxs);

		void bind();
		void unbind();

		void setData(std::vector<unsigned int> const& data);
		void resize(size_t size);

		void draw();

		void dispose();

	};

	extern "C" __declspec(dllexport) impl::IndexBuffer_base * createIndexBuffer_1(std::vector<unsigned int> const& idxs);
}
