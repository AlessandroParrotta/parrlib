#pragma once

#include <string>
#include <vector>

#include "../parrlib/impl/impl.h"

namespace prb {
	class FrameBuffer {
	public:
		impl::FrameBuffer_base* ptr;

		FrameBuffer();
		FrameBuffer(vec2 res);

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

}
