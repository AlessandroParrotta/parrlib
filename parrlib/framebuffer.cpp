#include "framebuffer.h"

namespace prb {
	FrameBuffer::FrameBuffer() {}
	FrameBuffer::FrameBuffer(vec2 res) { ptr = impl::createFrameBuffer_2(res); }

	void FrameBuffer::resize(vec2 res) { ptr->resize(res); }

	void FrameBuffer::clear(vec4 color) { ptr->clear(color); }
	void FrameBuffer::clear() { clear(vc4::black); }

	void FrameBuffer::bind() { ptr->bind(); }
	void FrameBuffer::unbind() { ptr->unbind(); }

	void FrameBuffer::drawImmediate(mat3 const& mat) { ptr->drawImmediate(mat); }

	void FrameBuffer::bindTexture() { ptr->bindTexture(); }
	void FrameBuffer::unbindTexture() { ptr->unbindTexture(); }

	void FrameBuffer::dispose() { ptr->dispose(); delete ptr; }

}
