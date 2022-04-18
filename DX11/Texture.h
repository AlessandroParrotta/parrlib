#pragma once

#include <string>
#include <vector>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <parrlibcore/vector2f.h>

#include "../parrlib/impl/texture.h"

namespace dx11 {
	using namespace prb;

	class Texture : public impl::Texture_base {
		void defInit();
		void init(unsigned char* data, vec2 size);

	public:
		ID3D11Texture2D* texture = NULL;
		ID3D11ShaderResourceView* resView = NULL;
		ID3D11SamplerState* sampler = NULL;

		unsigned char* data = NULL;

		std::wstring path = L"";

		int width = 0, height = 0, linesize = 0;

		Texture();
		Texture(std::wstring const& path);
		Texture(unsigned char* data, vec2 size);
		Texture(unsigned char* data, vec2 size, int linesize);
		Texture(vec2 size);
		Texture(vec2 size, int linesize);

		void fillRegion(unsigned char* subData, vec2 start, vec2 size);
		std::vector<unsigned char> getData() const;

		void resize(vec2 newSize);
		vec2 getSize() const;

		void setData(unsigned char* data, vec2 size, int linesize);
		void setData(unsigned char* data, vec2 size);

		//extremely inefficient, DO NOT USE unless it's for testing
		//this assumes there is already a shader bound to the device
		//vertex layout should be {posX, posY, colorR, colorG, colorB, colorA, uvX, uvY}
		void drawImmediate(std::vector<float> const& verts) const;

		//extremely inefficient, DO NOT USE unless it's for testing
		//this assumes there is already a shader bound to the device
		void drawImmediate(vec2 pos, vec2 size) const;

		void bind() const;

		void dispose();

		bool null() const;
	};

	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_1()												{ return new Texture(); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_2(std::wstring const& path)						{ return new Texture(path); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_3(unsigned char* data, vec2 size)				{ return new Texture(data, size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_4(unsigned char* data, vec2 size, int linesize) { return new Texture(data, size, linesize); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_5(vec2 size)									{ return new Texture(size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_6(vec2 size, int linesize)						{ return new Texture(size, linesize); }
	
	//extern "C" __declspec(dllexport) void disposeTexture(impl::Texture_base* t) {
	//	t->dispose();
	//	delete t;
	//}
}
