#pragma once

#include "../math/vector2f.h"
#include "../math/vector4f.h"
#include "../math/matrix3f.h"

namespace prb {
	namespace impl {
		class FrameBuffer_base {
		public:
			//virtual ~FrameBuffer_base();
			virtual void resize(vec2 res) = 0;

			virtual void clear(vec4 color) = 0;
			virtual void clear() = 0;

			virtual void bind() = 0;
			virtual void unbind() = 0;

			virtual void bindTexture() = 0;
			virtual void unbindTexture() = 0;

			virtual void drawImmediate(mat3 const& mat) = 0;

			virtual void dispose() = 0;
		};
	}
}
