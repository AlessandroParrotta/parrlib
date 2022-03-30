#pragma once

#include "../parrlib/math/vector2f.h"
#include "../parrlib/math/vector4f.h"
#include "../parrlib/math/matrix3f.h"

#include "../parrlib/impl/framebuffer.h"

namespace softst {
	using namespace prb;
	class FrameBuffer : public impl::FrameBuffer_base{
	public:
		unsigned char* data; //rgba
		vec2 size;

		FrameBuffer();
		FrameBuffer(vec2 size);

		void resize(vec2 res);

		void clear(vec4 color);
		void clear();

		void bind();
		void unbind();

		void bindTexture();
		void unbindTexture();

		void drawImmediate(mat3 const& mat);

		void dispose();
	};
	typedef FrameBuffer fbuf;

	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_1();
	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_2(vec2 size);

	//template<typename PixFmt, int Channels>
	//class FrameBuffer {

	//};

	//template<typename PixFmt, int Channels> using fbuf = FrameBuffer<PixFmt, Channels>;

}