#pragma once

#include <string>
#include <windows.h>

#include "texture.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "framebuffer.h"
#include "shader.h"
#include "impl_context.h"

namespace prb {
	namespace impl {

		extern HMODULE backendModule;

		FARPROC throwIfErr(HMODULE hModule, LPCSTR name);

		//_--------------------------TEXTURE-----------------------_//

		typedef Texture_base* (*CREATETEXTURE1)();												extern CREATETEXTURE1 createTexture_1;
		typedef Texture_base* (*CREATETEXTURE2)(std::wstring const& path);						extern CREATETEXTURE2 createTexture_2;
		typedef Texture_base* (*CREATETEXTURE3)(unsigned char* data, vec2 size);				extern CREATETEXTURE3 createTexture_3;
		typedef Texture_base* (*CREATETEXTURE4)(unsigned char* data, vec2 size, int linesize);	extern CREATETEXTURE4 createTexture_4;
		typedef Texture_base* (*CREATETEXTURE5)(vec2 size);										extern CREATETEXTURE5 createTexture_5;
		typedef Texture_base* (*CREATETEXTURE6)(vec2 size, int linesize);						extern CREATETEXTURE6 createTexture_6;

		void loadTextureFuncs();

		//_---------------------------VERTEXBUFFER-------------------------_//

		typedef VertexBuffer_base* (*CREATEVERTEXBUFFER1)();																			extern CREATEVERTEXBUFFER1 createVertexBuffer_1;
		typedef VertexBuffer_base* (*CREATEVERTEXBUFFER2)(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, unsigned int offset);		extern CREATEVERTEXBUFFER2 createVertexBuffer_2;
		typedef VertexBuffer_base* (*CREATEVERTEXBUFFER3)(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, unsigned int offset);	extern CREATEVERTEXBUFFER3 createVertexBuffer_3;
		typedef VertexBuffer_base* (*CREATEVERTEXBUFFER4)(size_t byteSize, std::vector<unsigned int> const& dataTypes, unsigned int offset);					extern CREATEVERTEXBUFFER4 createVertexBuffer_4;

		void loadVertexBufferFuncs();

		//_---------------------------INDEXBUFFER-------------------------_//

		typedef IndexBuffer_base* (*CREATEINDEXBUFFER1)(std::vector<unsigned int> const& idxs);		extern CREATEINDEXBUFFER1 createIndexBuffer_1;

		void loadIndexBufferFuncs();

		//_---------------------------FRAMEBUFFER-------------------------_//

		typedef FrameBuffer_base* (*CREATEFRAMEBUFFER1)();				extern CREATEFRAMEBUFFER1 createFrameBuffer_1;
		typedef FrameBuffer_base* (*CREATEFRAMEBUFFER2)(vec2 res);		extern CREATEFRAMEBUFFER2 createFrameBuffer_2;

		void loadFrameBufferFuncs();

		//_---------------------------SHADER-------------------------_//

		typedef Shader_base* (*CREATESHADER1)(std::string const& name, std::vector<std::pair<std::string, std::string>> attributes); extern CREATESHADER1 createShader_1;

		void loadShaderFuncs();

		//_---------------------------CONTEXT-------------------------_//

		void loadContextFuncs();

		void init(std::wstring dllName);

		void destroy();
	}
}
