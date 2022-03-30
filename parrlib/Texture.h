#pragma once

#include "impl/impl.h"

namespace prb {
	class Texture {
	public:
		impl::Texture_base* ptr = NULL;

		Texture();
		Texture(std::wstring const& path);
		Texture(const wchar_t* path);
		Texture(const char* path);
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
		void bind() const;
		void drawImmediate(vec2 pos, vec2 size) const;
		void dispose();

		bool null() const;
	};

}
