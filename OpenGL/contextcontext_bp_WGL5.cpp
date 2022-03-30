#include "context.h"

#include "../parrlib/math/stringutils.h"
#include "../parrlib/math/otherutil.h"
#include "../parrlib/debug.h"
#include "../parrlib/context.h"

#include "common.h"

#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <glad/glad.h>
//#include <gl/GLU.h>
//#include "wglext.h"

namespace gl {
    using namespace prb;
	namespace GLContext {

        HDC dc;
        HGLRC rc;
        HWND hWnd;

		bool binitialized = false;
		//GLFWwindow* window;
		vec2 res;

		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {

            const wchar_t CLASS_NAME[] = L"WindowClass";

            //WNDCLASS wc = { 0 };

            //const wchar_t CLASS_NAME[] = L"Tutorial";
            //wc.lpszClassName = CLASS_NAME;
            //wc.lpfnWndProc = WindowProc;
            //wc.hInstance = hInstance;

            //RegisterClass(&wc);

            // create window

            HMODULE oglhm = LoadLibrary(L"opengl32.dll");
            if (!oglhm) std::terminate();

            int width = bb.sizex();
            int height = bb.sizey();
            RECT wr = { bb[0].x, bb[0].y, width, height };
            AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

            HWND hWnd = CreateWindowEx(CS_OWNDC,
                L"WindowClass",
                title.c_str(),
                WS_OVERLAPPEDWINDOW/* | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/,
                0, 0,
                wr.right - wr.left,
                wr.bottom - wr.top,
                NULL,
                NULL,
                hInstance,
                NULL
            );
            
            //CreateWindow(CLASS_NAME, title.c_str(),
            //    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            //    0, 0,
            //    wr.right - wr.left,
            //    wr.bottom - wr.top,
            //    0, 0, hInstance, 0);

            if (hWnd == NULL)
            {
                return 0;
            }

            //PIXELFORMATDESCRIPTOR pfd = {
            //        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
            //        1,                     // version number  
            //        PFD_DRAW_TO_WINDOW |   // support window  
            //            PFD_SUPPORT_OPENGL |   // support OpenGL  
            //            PFD_DOUBLEBUFFER,      // double buffered  
            //        PFD_TYPE_RGBA,         // RGBA type  
            //        24,                    // 24-bit color depth  
            //        0, 0, 0, 0, 0, 0,      // color bits ignored  
            //        0,                     // no alpha buffer  
            //        0,                     // shift bit ignored  
            //        0,                     // no accumulation buffer  
            //        0, 0, 0, 0,            // accum bits ignored  
            //        32,                    // 32-bit z-buffer      
            //        0,                     // no stencil buffer  
            //        0,                     // no auxiliary buffer  
            //        PFD_MAIN_PLANE,        // main layer  
            //        0,                     // reserved  
            //        0, 0, 0                // layer masks ignored  
            //};
            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory(&pfd, sizeof(pfd));
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 24;

            dc = GetDC(hWnd); if (!dc) std::terminate();
            int pf = ChoosePixelFormat(dc, &pfd); //wglChoosePixelFormatARB(...)
            //com::getLastError(false); //consume the seemengly useless error that ChoosePixelFormat throws...

            if (!SetPixelFormat(dc, pf, &pfd)) std::terminate();

            rc = wglCreateContext(dc); if (rc == NULL) { deb::msberr("could not create WGL context"); std::terminate(); }

            if (!wglMakeCurrent(dc, rc)) std::terminate();
            if (!gladLoadGL()) { deb::msberr("error initializing GLAD"); }

            //if (com::getLastError()) std::terminate();  //for some reason ChoosePixelFormat throws an error, but if i ignore it GL still opens and works flawlessly...?????
            if (com::checkForGLErrors()) std::terminate();

            deb::debss << "OpenGL Version: " << GLVersion.major << " " << GLVersion.minor  << " " << glGetString(GL_VERSION) << "\n"; deb::outStr();

            RECT rect;  GetClientRect(hWnd, &rect);
            glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);

            //ReleaseDC(hWnd, dc);

            //InvalidateRect(hWnd, NULL, NULL);

            //dc = GetDC(hWnd);

            //ShowWindow(hWnd, SW_SHOW);

			//if (com::getLastError()) std::terminate();

			//if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) { deb::debss << "Failed to initialize GLAD"; deb::msberr(); std::terminate(); }

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);

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
			//if (com::checkForGLErrors()) std::terminate();
		}

		void present(bool vSync) {
            //SwapBuffers(viewwnd::hDC);
			SwapBuffers(dc);
		}

		void dispose() {
            //OnOpenGLWindowDestroy();
            wglMakeCurrent(NULL, NULL);
            ReleaseDC(hWnd, dc);
            DestroyWindow(hWnd);
		}

		void setFullscreen(bool fullscreen) {
			//TODO
		}

		void resize(vec2 res) {
			GLContext::res = res;
            glViewport(0, 0, res.x, res.y);
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
