#include "statemachine.h"

namespace softst {
	namespace statemachine {
		fbuf const* backBuffer = NULL;

		fbuf const* curFbuf = NULL;
		fbuf const* currentFrameBuffer() { return curFbuf; }
		void bindFrameBuffer(fbuf const* fb) { curFbuf = fb; }
		void unbindFrameBuffer() { bindFrameBuffer(backBuffer); }

		Texture const* curTex = NULL;
		Texture const* currentTexture() { return curTex; }
		void bindTexture(Texture const * tex) { curTex = tex; }
		void unbindTexture() { curTex = NULL; }

		VertexBuffer const* curVb = NULL;
		VertexBuffer const* currentVertexBuffer() { return curVb; }
		void bindVertexBuffer(VertexBuffer const* vb) { curVb = vb; }
		void unbindVertexBuffer() { curVb = NULL; }

		Shader* curVertexShader = NULL;
		Shader* currentVertexShader() { return curVertexShader; }
		void bindVertexShader(Shader* sh) { curVertexShader = sh; }
		void unbindVertexShader() { curVertexShader = NULL; }

		Shader* curFragmentShader = NULL;
		Shader* currentFragmentShader() { return curFragmentShader; }
		void bindFragmentShader(Shader* sh) { curFragmentShader = sh; }
		void unbindFragmentShader() { curFragmentShader = NULL; }
	}
}
