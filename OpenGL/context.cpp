#include "context.h"

#include "../parrlib/math/stringutils.h"
#include "../parrlib/math/otherutil.h"
#include "../parrlib/debug.h"
#include "../parrlib/context.h"


#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <glad/glad.h>
//#include <gl/GLU.h>

#include "wglext.h"
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#include "common.h"

namespace gl {
    using namespace prb;
	namespace GLContext {

        int dwExStyle = WS_EX_APPWINDOW;
        int dwStyle = WS_OVERLAPPEDWINDOW;

        HINSTANCE hInstance;
        std::wstring title;
        aabb2 bb;

        int pxfmt = 0;
        HDC dc;
        HGLRC rc;
        HWND hWnd;

		bool binitialized = false;
		//GLFWwindow* window;
		vec2 res;
        bool vSync = true;
        bool fullscreen = false;

        void destroyWindow(HWND hWnd, HDC dc, HGLRC rc) {
            wglDeleteContext(rc);
            ReleaseDC(hWnd, dc);
            DestroyWindow(hWnd);
        }

        LRESULT CALLBACK FakeWndProc(HWND hwnd,
            UINT msg,
            WPARAM wParam,
            LPARAM lParam)
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);

        }

        PROC throwIfErr(LPCSTR str) { PROC res; if (!(res = wglGetProcAddress(str))) std::terminate(); return res; }

        struct fakeContext {
            HWND hWnd;
            HDC dc;
            HGLRC rc;
            int pf;
        };
        fakeContext createFakeContextAndLoadExtensions(HINSTANCE hInstance) {
            WNDCLASS wc = { 0 };

            const wchar_t CLASS_NAME[] = L"Tutorial2";
            wc.lpszClassName = CLASS_NAME;
            wc.lpfnWndProc = FakeWndProc;
            wc.hInstance = hInstance;

            RegisterClass(&wc);

            hWnd = CreateWindowEx(CS_OWNDC,
                CLASS_NAME,
                L"test",
                WS_OVERLAPPEDWINDOW/* | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/,
                0, 0,
                10,
                10,
                NULL,
                NULL,
                hInstance,
                NULL
            );

            HDC dc = GetDC(hWnd); if (!dc) std::terminate();

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory(&pfd, sizeof(pfd));
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 24;

            int pf = ChoosePixelFormat(dc, &pfd); if (!pf) std::terminate();
            if (!SetPixelFormat(dc, pf, &pfd)) std::terminate();

            HGLRC rc = wglCreateContext(dc); if (!rc) std::terminate();

            if (!wglMakeCurrent(dc, rc)) std::terminate();

            //const char* extensions = (char*)glGetString(GL_EXTENSIONS);
            //if (!extensions) std::terminate();

            //msbinfo(extensions);

            if (!gladLoadGL()) std::terminate();

            wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)throwIfErr("wglCreateContextAttribsARB");
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)throwIfErr("wglChoosePixelFormatARB");
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)throwIfErr("wglSwapIntervalEXT");

            return { hWnd, dc, rc, pf };
        }

        HWND createWGLEXTWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {
            GLContext::hInstance = hInstance;
            GLContext::title = title;
            GLContext::bb = bb;

            // create window

            int width = bb.sizex();
            int height = bb.sizey();
            RECT wr = { bb[0].x, bb[0].y, width, height };
            AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

            HWND hWnd = CreateWindowEx(dwExStyle,
                L"WindowClass",
                title.c_str(),
                dwStyle, // WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                0, 0,
                wr.right - wr.left,
                wr.bottom - wr.top,
                NULL,
                NULL,
                hInstance,
                NULL
            );

            if (hWnd == NULL) return 0;

            dc = GetDC(hWnd); if (!dc) std::terminate();

            int attribs[] = {
                WGL_DRAW_TO_WINDOW_ARB, TRUE,
                WGL_DOUBLE_BUFFER_ARB, TRUE,
                WGL_SUPPORT_OPENGL_ARB, TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 24,
                WGL_RED_BITS_ARB, 8,
                WGL_GREEN_BITS_ARB, 8,
                WGL_BLUE_BITS_ARB, 8,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                0, 0
            };
            UINT fmtNum = 0;
            if (!wglChoosePixelFormatARB(dc, attribs, NULL, 1, &pxfmt, &fmtNum)) std::terminate();

            if (fmtNum <= 0) std::terminate();

            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory(&pfd, sizeof(pfd));
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 24;

            if (!DescribePixelFormat(dc, pxfmt, sizeof(pfd), &pfd)) std::terminate(); //fake call probably to invoke symbols in dll

            if (!SetPixelFormat(dc, pxfmt, &pfd)) std::terminate();

            int context_attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_PROFILE_MASK_ARB, /*WGL_CONTEXT_CORE_PROFILE_BIT_ARB | */WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0, 0
            };
            rc = wglCreateContextAttribsARB(dc, NULL, context_attribs); if (!rc) std::terminate();

            return hWnd;
        }

		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {

            fakeContext fctx = createFakeContextAndLoadExtensions(hInstance);
            pxfmt = fctx.pf;

            hWnd = createWGLEXTWindow(hInstance, title, bb);

            if (!wglMakeCurrent(NULL, NULL)) std::terminate();
            wglDeleteContext(fctx.rc);
            ReleaseDC(fctx.hWnd, fctx.dc);
            DestroyWindow(fctx.hWnd);

            if (!wglMakeCurrent(dc, rc)) std::terminate();

            //PIXELFORMATDESCRIPTOR pfd;
            //ZeroMemory(&pfd, sizeof(pfd));
            //pfd.nSize = sizeof(pfd);
            //pfd.nVersion = 1;
            //pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            //pfd.iPixelType = PFD_TYPE_RGBA;
            //pfd.cColorBits = 24;


            //dc = GetDC(hWnd); if (!dc) std::terminate();


            //int pf = ChoosePixelFormat(dc, &pfd); if (!pf) std::terminate(); //wglChoosePixelFormatARB(...)

            //if (!SetPixelFormat(dc, pf, &pfd)) std::terminate();

            //rc = wglCreateContext(dc); if (rc == NULL) { msberr("could not create WGL context"); std::terminate(); }

            //if (!wglMakeCurrent(dc, rc)) std::terminate();
            if (!gladLoadGL()) { deb::mbe("error initializing GLAD"); }

            if (!wglSwapIntervalEXT(true)) std::terminate();

            SetLastError(0);

            if (com::getLastError()) std::terminate();
            if (com::checkForGLErrors()) std::terminate();

            deb::tss << "OpenGL Version: " << GLVersion.major << " " << GLVersion.minor  << " " << glGetString(GL_VERSION) << "\n"; 
            deb::outStr();

            RECT rect;  GetClientRect(hWnd, &rect);
            glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);

            //ReleaseDC(hWnd, dc);

            //InvalidateRect(hWnd, NULL, NULL);

            //dc = GetDC(hWnd);

            //ShowWindow(hWnd, SW_SHOW);

			//if (com::getLastError()) std::terminate();

			//if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) { deb::debss << "Failed to initialize GLAD"; deb::msberr(); std::terminate(); }

            if (!wglSwapIntervalEXT(vSync)) std::terminate();

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
            if (vSync != GLContext::vSync) { if (!wglSwapIntervalEXT(vSync)) std::terminate(); GLContext::vSync = vSync; }

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
            if (GLContext::fullscreen != fullscreen) { 
                //if (!wglMakeCurrent(NULL, NULL)) std::terminate();
                //destroyWindow(hWnd, dc, rc); 

                if (prc::dDevices.size() == 0) prc::dDevices = prc::getDisplayDevices();
                if (prc::dDevices.size() == 0) { deb::mbe("No display device found! (GL)"); std::terminate(); }

                prc::displayDevice::deviceMode nativeDev = prc::dDevices[0].getNative();
                deb::tss << nativeDev.res << " " << nativeDev.refreshRate << "\n"; deb::outStr();

                if (fullscreen) {

                    //DEVMODE dmScreenSettings;                               // Device Mode
                    ////memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // Makes Sure Memory's Cleared
                    //ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
                    //dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
                    //dmScreenSettings.dmPelsWidth = 1920;                // Selected Screen Width
                    //dmScreenSettings.dmPelsHeight = 1080;               // Selected Screen Height
                    //dmScreenSettings.dmBitsPerPel = 32;                 // Selected Bits Per Pixel - Win8 and later only support 32+ bpp 
                    //dmScreenSettings.dmDisplayFrequency = 144;
                    //dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

                    SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
                    SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
                    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, nativeDev.res.x, nativeDev.res.y, SWP_SHOWWINDOW);

                    // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
                    // CDS fullscreen is for games (it's temporary, not global and permanent)
                    if (ChangeDisplaySettingsEx(prc::dDevices[0].name.c_str(), &nativeDev.rawMode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) std::terminate();
                }
                else{
                    SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
                    SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
                    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, nativeDev.res.x, nativeDev.res.y, SWP_SHOWWINDOW);

                    if (ChangeDisplaySettingsEx(prc::dDevices[0].name.c_str(), NULL, NULL, NULL, NULL) != DISP_CHANGE_SUCCESSFUL) std::terminate();
                }

                //if (fullscreen) {
                //    dwExStyle = WS_EX_APPWINDOW;                              // Window Extended Style
                //    dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;   // Windows Style
                //}
                //else{
                //    dwExStyle = WS_EX_APPWINDOW;                       // Window Extended Style
                //    dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;    // Windows Style
                //}

                //hWnd = createWGLEXTWindow(hInstance, title, bb);
                //if (!hWnd) std::terminate();

                //deb::debss << hWnd << "\n"; deb::msbinfo();

                GLContext::fullscreen = fullscreen; 
            }
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
