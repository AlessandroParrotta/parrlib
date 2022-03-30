#include "impl.h"

#include "../debug.h"
#include "../math/stringutils.h"

namespace prb {
	namespace impl {
		HMODULE backendModule;

		FARPROC throwIfErr(HMODULE hModule, LPCSTR name) {
			FARPROC res = GetProcAddress(hModule, name);
			if (res == NULL) {
				deb::tss << "error loading DLL function '" << name << "'"; deb::mbe();
				std::terminate();
			}
			return res;
		}

		//_--------------------------TEXTURE-----------------------_//

		CREATETEXTURE1 createTexture_1;
		CREATETEXTURE2 createTexture_2;
		CREATETEXTURE3 createTexture_3;
		CREATETEXTURE4 createTexture_4;
		CREATETEXTURE5 createTexture_5;
		CREATETEXTURE6 createTexture_6;

		void loadTextureFuncs() {
			createTexture_1 = (CREATETEXTURE1)throwIfErr(backendModule, "createTexture_1");
			createTexture_2 = (CREATETEXTURE2)throwIfErr(backendModule, "createTexture_2");
			createTexture_3 = (CREATETEXTURE3)throwIfErr(backendModule, "createTexture_3");
			createTexture_4 = (CREATETEXTURE4)throwIfErr(backendModule, "createTexture_4");
			createTexture_5 = (CREATETEXTURE5)throwIfErr(backendModule, "createTexture_5");
			createTexture_6 = (CREATETEXTURE6)throwIfErr(backendModule, "createTexture_6");
		}

		//_---------------------------VERTEXBUFFER-------------------------_//

		CREATEVERTEXBUFFER1 createVertexBuffer_1;
		CREATEVERTEXBUFFER2 createVertexBuffer_2;
		CREATEVERTEXBUFFER3 createVertexBuffer_3;
		CREATEVERTEXBUFFER4 createVertexBuffer_4;

		void loadVertexBufferFuncs() {
			createVertexBuffer_1 = (CREATEVERTEXBUFFER1)throwIfErr(backendModule, "createVertexBuffer_1");
			createVertexBuffer_2 = (CREATEVERTEXBUFFER2)throwIfErr(backendModule, "createVertexBuffer_2");
			createVertexBuffer_3 = (CREATEVERTEXBUFFER3)throwIfErr(backendModule, "createVertexBuffer_3");
			createVertexBuffer_4 = (CREATEVERTEXBUFFER4)throwIfErr(backendModule, "createVertexBuffer_4");
		}

		//_---------------------------INDEXBUFFER-------------------------_//

		typedef IndexBuffer_base* (*CREATEINDEXBUFFER1)(std::vector<unsigned int> const& idxs);		CREATEINDEXBUFFER1 createIndexBuffer_1;
		
		void loadIndexBufferFuncs() {
			createIndexBuffer_1 = (CREATEINDEXBUFFER1)throwIfErr(backendModule, "createIndexBuffer_1");
		}

		//_---------------------------FRAMEBUFFER-------------------------_//

		CREATEFRAMEBUFFER1 createFrameBuffer_1;
		CREATEFRAMEBUFFER2 createFrameBuffer_2;

		void loadFrameBufferFuncs() {
			createFrameBuffer_1 = (CREATEFRAMEBUFFER1)throwIfErr(backendModule, "createFrameBuffer_1");
			createFrameBuffer_2 = (CREATEFRAMEBUFFER2)throwIfErr(backendModule, "createFrameBuffer_2");
		}

		//_---------------------------SHADER-------------------------_//

		CREATESHADER1 createShader_1;

		void loadShaderFuncs() {
			createShader_1 = (CREATESHADER1)throwIfErr(backendModule, "createShader_1");
		}

		//_---------------------------CONTEXT-------------------------_//

		void loadContextFuncs() {
			context::createWindow = (context::CREATEWINDOW)throwIfErr(backendModule, "createWindow");

			context::init = (context::INIT)throwIfErr(backendModule, "init");
			context::initialized = (context::INITIALIZED)throwIfErr(backendModule, "initialized");

			context::setBlend = (context::SETBLEND)throwIfErr(backendModule, "setBlend");

			context::clear = (context::CLEAR)throwIfErr(backendModule, "clear");

			context::present = (context::PRESENT)throwIfErr(backendModule, "present");

			context::dispose = (context::DISPOSE)throwIfErr(backendModule, "dispose");

			context::setFullscreen = (context::SETFULLSCREEN)throwIfErr(backendModule, "setFullscreen");

			context::resize = (context::RESIZE)throwIfErr(backendModule, "resize");
		}

		void init(std::wstring dllName) {
			backendModule = LoadLibrary(stru::fallbackPath((L"backend\\" + dllName)).c_str());
			if (!backendModule) { deb::mbe(L"could not load backend module '" + dllName + L"'"); std::terminate();  return; }
			//if (!backendModule) { std::terminate(); return; }

			loadTextureFuncs();
			loadVertexBufferFuncs();
			loadIndexBufferFuncs();
			loadFrameBufferFuncs();
			loadShaderFuncs();
			loadContextFuncs();
		}

		void destroy() {
			//FreeLibrary(backendModule);
			if (!FreeLibrary(backendModule)) deb::mbe(L"could not free backend module");
		}
	}
}
