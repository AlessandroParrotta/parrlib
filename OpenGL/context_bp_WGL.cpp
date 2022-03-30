#include "context.h"

#include "../parrlib/math/stringutils.h"
#include "../parrlib/debug.h"
#include "../parrlib/context.h"

#include "common.h"

#define VIEWCLASS   "ViewWnd"
#define VIEWSTYLE   WS_VISIBLE|WS_POPUP|WS_MAXIMIZE

#include <windows.h>
#include <glad/glad.h>
//#include <gl/GLU.h>
#include "wgltext.h"

namespace gl {
	namespace GLContext {

        namespace viewwnd
        {
            HWND    hWnd;
            HDC     hDC;
            HGLRC   hRC;
        };

        namespace render
        {
            int win_width;
            int win_height;
        }

        typedef HGLRC(*wglprocCreateContextAttribsARB)(HDC, HGLRC, const int*);
        typedef BOOL(*wglprocChoosePixelFormatARB)(HDC, const int*, const FLOAT*, UINT, int*, UINT*);

#define VIEWCLASS   L"ViewWnd"
#define VIEWSTYLE   WS_VISIBLE|WS_POPUP|WS_MAXIMIZE

        void reshape(int w, int h)
        {
            render::win_width = w;
            render::win_height = h;
        }

        void display()
        {
            using namespace viewwnd;
            using namespace render;

            //glViewport(0, 0, win_width, win_height);

            //glClearColor(0., 0., 0., 1.);
            //glClearDepth(1.);
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //glMatrixMode(GL_PROJECTION);
            //glLoadIdentity();
            //gluPerspective(45, 1., 0.1, 100);

            //glMatrixMode(GL_MODELVIEW);
            //glLoadIdentity();

            SwapBuffers(hDC);
        }

        void OnOpenGLWindowDestroy()
        {
            using namespace viewwnd;
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hRC);
            ReleaseDC(hWnd, hDC);
            UnregisterClass(VIEWCLASS, GetModuleHandle(NULL));
            PostQuitMessage(0);
        }

        LRESULT CALLBACK ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            using namespace viewwnd;
            switch (uMsg)
            {
            case WM_DESTROY:
                OnOpenGLWindowDestroy();
                break;
            case WM_PAINT:
                display();
                break;
            case WM_SIZE:
                reshape(LOWORD(lParam), HIWORD(lParam));
                break;
            default:
                break;
            }
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        HWND OpenGLWindowCreate(HINSTANCE hInstance, std::wstring title, aabb2 bb)
        {
            srand((unsigned)time(NULL));
            using namespace viewwnd;
            {
                WNDCLASS wc;
                memset(&wc, 0, sizeof(wc));
                wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
                wc.lpfnWndProc = ViewProc;
                wc.hInstance = GetModuleHandle(NULL);
                wc.lpszClassName = VIEWCLASS;
                RegisterClass(&wc);
            }

            /* Create a temporaray context to get address of wgl extensions. */

            

            HWND hTempWnd = CreateWindowEx(WS_EX_APPWINDOW,
                VIEWCLASS,
                L"Simple",
                VIEWSTYLE,
                bb[0].x, bb[0].y, bb.sizex(), bb.sizey(),
                NULL, NULL,
                hInstance,
                NULL);
            if (!hTempWnd) {
                deb::debss << "could not create window hTempWnd"; deb::msberr(); std::terminate();
                return NULL;
            }

            deb::msbinfo("temp window created");

            HDC hTempDC = GetDC(hTempWnd);
            if (!hTempDC) {
                DestroyWindow(hTempWnd);
                deb::debss << "could not get hTempDC"; deb::msberr(); std::terminate();
                return NULL;
            }

            deb::msbinfo("got hTempDC");

            PIXELFORMATDESCRIPTOR pfd;
            memset(&pfd, 0, sizeof(pfd));
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.cColorBits = 32;
            pfd.cDepthBits = 24;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.iLayerType = PFD_MAIN_PLANE;

            HGLRC hTempRC;

            int iPF;
            if ((!(iPF = ChoosePixelFormat(hTempDC, &pfd)) || !SetPixelFormat(hTempDC, iPF, &pfd)) ||

                (!(hTempRC = wglCreateContext(hTempDC)) || !wglMakeCurrent(hTempDC, hTempRC))) {
                ReleaseDC(hTempWnd, hTempDC);
                DestroyWindow(hTempWnd);

                deb::debss << "could not get pixel format"; deb::msberr(); std::terminate();

                return NULL;
            }

            deb::msbinfo("got hTempRC");

            /* Like all extensions in Win32, the function pointers returned by wglGetProcAddress are tied
             * to the render context they were obtained with. Since this is a temporary context, we
             * place those function pointers in automatic storage of the window and context creation function. */
            wglprocCreateContextAttribsARB wglCreateContextAttribsARB = (wglprocCreateContextAttribsARB)wglGetProcAddress("wglCreateContextAttribsARB");
            wglprocChoosePixelFormatARB wglChoosePixelFormatARB = (wglprocChoosePixelFormatARB)wglGetProcAddress("wglChoosePixelFormatARB");

            if (wglChoosePixelFormatARB && wglCreateContextAttribsARB) {
                /* good we have access to extended pixelformat and context attributes */
                hWnd = CreateWindowEx(WS_EX_APPWINDOW,
                    VIEWCLASS,
                    L"Simple",
                    VIEWSTYLE,
                    bb[0].x, bb[0].y, bb.sizex(), bb.sizey(),
                    NULL, NULL,
                    hInstance,
                    NULL);

                if (!hWnd) {
                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(hTempRC);
                    ReleaseDC(hTempWnd, hTempDC);
                    DestroyWindow(hTempWnd);

                    deb::debss << "could not get hWnd"; deb::msberr(); std::terminate();

                    return NULL;
                }

                deb::msbinfo("created hWnd");

                hDC = GetDC(hWnd);
                if (!hDC) {
                    DestroyWindow(hWnd);

                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(hTempRC);
                    ReleaseDC(hTempWnd, hTempDC);
                    DestroyWindow(hTempWnd);

                    deb::debss << "could not get hDC"; deb::msberr(); std::terminate();

                    return NULL;
                }

                deb::msbinfo("created hDC");

                int attribs[] = {
                    WGL_DRAW_TO_WINDOW_ARB, TRUE,
                    WGL_DOUBLE_BUFFER_ARB, TRUE,
                    WGL_SUPPORT_OPENGL_ARB, TRUE,
                    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                    WGL_COLOR_BITS_ARB, 32,
                    WGL_RED_BITS_ARB, 8,
                    WGL_GREEN_BITS_ARB, 8,
                    WGL_BLUE_BITS_ARB, 8,
                    WGL_ALPHA_BITS_ARB, 8,
                    WGL_DEPTH_BITS_ARB, 24,
                    WGL_STENCIL_BITS_ARB, 8,
                    0, 0
                };
                UINT num_formats_choosen;
                BOOL choose_pf_success = wglChoosePixelFormatARB(
                    hDC,
                    attribs,
                    NULL,
                    1,
                    &iPF,
                    &num_formats_choosen);

                /* now this is a kludge; we need to pass something in the PIXELFORMATDESCRIPTOR
                 * to SetPixelFormat; it will be ignored, mostly. OTOH we want to send something
                 * sane, we're nice people after all - it doesn't hurt if this fails. */
                DescribePixelFormat(hDC, iPF, sizeof(pfd), &pfd);

                if (!(choose_pf_success &&
                    num_formats_choosen >= 1 &&
                    SetPixelFormat(hDC, iPF, &pfd))) {
                    ReleaseDC(hWnd, hDC);
                    DestroyWindow(hWnd);

                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(hTempRC);
                    ReleaseDC(hTempWnd, hTempDC);
                    DestroyWindow(hTempWnd);

                    deb::debss << "could not describe pixel format"; deb::msberr(); std::terminate();

                    return NULL;
                }

                deb::msbinfo("described pixel format");

                /* we request a OpenGL-3 compatibility profile */
                int context_attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0, 0
                };
                hRC = wglCreateContextAttribsARB(hDC, NULL, context_attribs);
                if (!hRC) {
                    ReleaseDC(hWnd, hDC);
                    DestroyWindow(hWnd);

                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(hTempRC);
                    ReleaseDC(hTempWnd, hTempDC);
                    DestroyWindow(hTempWnd);

                    deb::debss << "could not create wglCreateContextAttribsARB"; deb::msberr(); std::terminate();

                    return NULL;
                }
                wglMakeCurrent(hDC, hRC);

                deb::msbinfo("got hRC and made current");

                /* now that we've created the proper window, DC and RC
                 * we can delete the temporaries */
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(hTempRC);
                ReleaseDC(hTempWnd, hTempDC);
                DestroyWindow(hTempWnd);

                deb::msbinfo("destroyed window");
            }
            else {
                /* extended pixelformats and context attributes not supported
                 * => use temporary window and context as the proper ones */
                hWnd = hTempWnd;
                hDC = hTempDC;
                hRC = hTempRC;

                deb::msbinfo("using temp as main hWnds");
            }

            ShowWindow(hWnd, SW_SHOW);
            UpdateWindow(hWnd);

            wglMakeCurrent(hDC, hRC);

            return hWnd;
        }

		bool binitialized = false;
		//GLFWwindow* window;
		vec2 res;

		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {
            HWND wnd = OpenGLWindowCreate(hInstance, title, bb);
            if (!wnd) { deb::debss << "error creating window"; deb::msberr(); std::terminate(); }
            
            if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) { deb::debss << "Failed to initialize GLAD"; deb::msberr(); std::terminate(); }

            return wnd;
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
            SwapBuffers(viewwnd::hDC);
		}

		void dispose() {
            OnOpenGLWindowDestroy();
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
