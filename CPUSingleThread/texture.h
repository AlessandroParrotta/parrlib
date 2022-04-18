#pragma once

#include <parrlibcore/vector2f.h>

#include "../parrlib/impl/texture.h"

namespace softst {
	using namespace prb;
	class Texture : public impl::Texture_base{
	public:
		unsigned char* data;
		vec2 size;

		Texture();
		Texture(std::wstring const& path);
		Texture(unsigned char* data, vec2 size);
		Texture(unsigned char* data, vec2 size, int linesize);
		Texture(vec2 size);
		Texture(vec2 size, int linesize);

		void fillRegion(unsigned char* subData, vec2 start, vec2 end);
		std::vector<unsigned char> getData() const;

		void resize(vec2 newSize);
		vec2 getSize() const;

		void setData(unsigned char* data, vec2 size, int linesize);
		void setData(unsigned char* data, vec2 size);
		void drawImmediate(std::vector<float> const& verts) const;
		void drawImmediate(vec2 pos, vec2 size) const;
		void bind() const;
		void dispose();

		bool null() const;
	};

	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_1();
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_2(std::wstring const& path);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_3(unsigned char* data, vec2 size);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_4(unsigned char* data, vec2 size, int linesize);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_5(vec2 size);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_6(vec2 size, int linesize);
}
