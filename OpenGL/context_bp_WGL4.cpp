#include "context.h"

#include "../parrlib/math/stringutils.h"
#include "../parrlib/debug.h"
#include "../parrlib/context.h"

#include "common.h"

#include <windows.h>
#include <wingdi.h>
#include <glad/glad.h>
//#include <gl/GLU.h>
#include "wgltext.h"

namespace gl {
	namespace GLContext {

		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_CREATE:
			{
				PIXELFORMATDESCRIPTOR pfd =
				{
					sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
					PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
					32,                   // Colordepth of the framebuffer.
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					24,                   // Number of bits for the depthbuffer
					8,                    // Number of bits for the stencilbuffer
					0,                    // Number of Aux buffers in the framebuffer.
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
				};

				HDC ourWindowHandleToDeviceContext = GetDC(hWnd);

				if (com::getLastError()) std::terminate();


				int  letWindowsChooseThisPixelFormat;
				letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
				
				if (com::getLastError()) std::terminate();


				SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

				HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
				wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);
				if (com::getLastError()) std::terminate();
				if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) { deb::debss << "Failed to initialize GLAD"; deb::msberr(); std::terminate(); }


				MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);

				//wglMakeCurrent(ourWindowHandleToDeviceContext, NULL); Unnecessary; wglDeleteContext will make the context not current
				wglDeleteContext(ourOpenGLRenderingContext);
				PostQuitMessage(0);
			}
			break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			return 0;

		}

		bool binitialized = false;
		//GLFWwindow* window;
		vec2 res;

		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {
			
			MSG msg = { 0 };
			WNDCLASS wc = { 0 };
			wc.lpfnWndProc = WndProc;
			wc.hInstance = hInstance;
			wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
			wc.lpszClassName = L"oglversionchecksample";
			wc.style = CS_OWNDC;
			if (!RegisterClass(&wc))
				return NULL;
			HWND hWnd = CreateWindow(wc.lpszClassName, L"openglversioncheck", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, 0, 0, hInstance, 0);

			if (com::getLastError()) std::terminate();

			if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) { deb::debss << "Failed to initialize GLAD"; deb::msberr(); std::terminate(); }

            return hWnd;
		}

		void init(HWND hWnd, int antiAlias, vec2 res) {

			GLContext::res = res;

			binitialized = true;
		}
		bool initialized() { return binitialized; }

		void setBlend(bool enabled) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (com::checkForGLErrors()) std::terminate();
		}

		void clear(vec4 color) {
			glClearColor(color.x, color.y, color.z, color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (com::checkForGLErrors()) std::terminate();
		}

		void present(bool vSync) {
            //SwapBuffers(viewwnd::hDC);
			//SwapBuffers(hDC);
		}

		void dispose() {
            //OnOpenGLWindowDestroy();
		}

		void setFullscreen(bool fullscreen) {
			//TODO
		}

		void resize(vec2 res) {
			GLContext::res = res;
            //TODO
		}
	}

	extern "C" __declspec(dllexport) HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) { return GLContext::createWindow(hInstance, title, bb); }

	extern "C" __declspec(dllexport) void init(HWND hWnd, int antiAlias, vec2 res) { GLContext::init(hWnd, antiAlias, res); }
	extern "C" __declspec(dllexport) bool initialized() { return GLContext::initialized(); }

	extern "C" __declspec(dllexport) void setBlend(bool enabled) { GLContext::setBlend(enabled); }

	extern "C" __declspec(dllexport) void clear(vec4 color) { GLContext::clear(color); }

	extern "C" __declspec(dllexport) void present(bool vSync) { GLContext::present(vSync); }

	extern "C" __declspec(dllexport) void dispose() { GLContext::dispose(); }

	extern "C" __declspec(dllexport) void setFullscreen(bool fullscreen) { GLContext::setFullscreen(fullscreen); }

	extern "C" __declspec(dllexport) void resize(vec2 res) { GLContext::resize(res); }
}
