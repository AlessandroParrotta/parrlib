#pragma once

#include "framebuffer.h"
#include "texture.h"
#include "vertexbuffer.h"
#include "shader.h"

namespace softst {
	namespace statemachine {
		extern fbuf const* backBuffer;
		fbuf const* currentFrameBuffer();
		void bindFrameBuffer(fbuf const* fb);
		void unbindFrameBuffer();

		Texture const* currentTexture();
		void bindTexture(Texture const* tex);
		void unbindTexture();

		VertexBuffer const* currentVertexBuffer();
		void bindVertexBuffer(VertexBuffer const* vb);
		void unbindVertexBuffer();

		Shader* currentVertexShader();
		void bindVertexShader(Shader* sh);
		void unbindVertexShader();

		Shader* currentFragmentShader();
		void bindFragmentShader(Shader* sh);
		void unbindFragmentShader();
	}
}
