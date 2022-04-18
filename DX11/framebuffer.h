#pragma once

#include <string>
#include <vector>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <parrlibcore/vector2f.h>
#include <parrlibcore/vector4f.h>
#include <parrlibcore/matrix3f.h>

#include "../parrlib/impl/framebuffer.h"

namespace dx11 {
	using namespace prb;

	class FrameBuffer : public impl::FrameBuffer_base{
	public:
		ID3D11Texture2D* texture = NULL;
		ID3D11ShaderResourceView* resView = NULL;
		ID3D11SamplerState* sampler = NULL;

		ID3D11Texture2D* depthStencilTex = NULL;
		ID3D11DepthStencilState* depthStencilState = NULL;
		ID3D11DepthStencilView* depthStencilView = NULL;

		ID3D11RenderTargetView* rtv;

		D3D11_VIEWPORT viewport;

		vec2 res;
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

		void defInit();

		FrameBuffer();
		FrameBuffer(vec2 res, DXGI_FORMAT format);
		FrameBuffer(vec2 res);

		void resize(vec2 res);

		void clear(vec4 color);
		void clear();

		void bind();
		void unbind();

		void bindTexture();
		void unbindTexture();

		void bindTex();
		void drawImmediate(mat3 const& mat);

		void dispose();
	};

	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_1();
	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_2(vec2 size);

	typedef FrameBuffer fbuf;

}
