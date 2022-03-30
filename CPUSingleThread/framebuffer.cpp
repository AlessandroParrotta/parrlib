#include "framebuffer.h"

#include "statemachine.h"

namespace softst {
	
	FrameBuffer::FrameBuffer(){}
	FrameBuffer::FrameBuffer(vec2 size) {
		this->data = new unsigned char[size.x * size.y * 4];
	}

	void FrameBuffer::resize(vec2 res){
		delete[] data;
		this->data = new unsigned char[size.x * size.y * 4];
	}

	void FrameBuffer::clear(vec4 color) {
		for (int i = 0; i < size.x*size.y*4; i+=4) {
			data[i + 0] = color.x;
			data[i + 1] = color.y;
			data[i + 2] = color.z;
			data[i + 3] = color.w;
		}
	}
	void FrameBuffer::clear() { clear(0.f); }

	void FrameBuffer::bind() { statemachine::bindFrameBuffer(this); }
	void FrameBuffer::unbind() { statemachine::unbindFrameBuffer(); }

	void FrameBuffer::bindTexture(){}
	void FrameBuffer::unbindTexture(){}

	void FrameBuffer::drawImmediate(mat3 const& mat){}

	void FrameBuffer::dispose() { delete[] data; }

	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_1() { return new FrameBuffer(); }
	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_2(vec2 size) { return new FrameBuffer(size); }
}