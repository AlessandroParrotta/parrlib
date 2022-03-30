#include "context.h"

#include "../parrlib/math/stringutils.h"
#include "../parrlib/debug.h"
#include "../parrlib/context.h"

#include "common.h"

#include <windows.h>
#include <glad/glad.h>
//#include <gl/GLU.h>
#include "wgltext.h"

namespace gl {
	namespace GLContext {

		static	HGLRC hRC;		// Permanent Rendering Context
		static	HDC hDC;		// Private GDI Device Context

		BOOL	keys[256];		// Array Used For The Keyboard Routine

		GLvoid InitGL(GLsizei Width, GLsizei Height)	// This Will Be Called Right After The GL Window Is Created
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
			glClearDepth(1.0);							// Enables Clearing Of The Depth Buffer
			glDepthFunc(GL_LESS);						// The Type Of Depth Test To Do
			glEnable(GL_DEPTH_TEST);					// Enables Depth Testing
			glShadeModel(GL_SMOOTH);					// Enables Smooth Color Shading

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();							// Reset The Projection Matrix

			//gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);	// Calculate The Aspect Ratio Of The Window

			glMatrixMode(GL_MODELVIEW);
		}

		GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
		{
			if (Height == 0)								// Prevent A Divide By Zero If The Window Is Too Small
				Height = 1;

			glViewport(0, 0, Width, Height);			// Reset The Current Viewport And Perspective Transformation

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			//gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
			glMatrixMode(GL_MODELVIEW);
		}

		GLvoid DrawGLScene(GLvoid)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
			glLoadIdentity();										// Reset The View
		}

		LRESULT CALLBACK WndProc(HWND	hWnd,
			UINT	message,
			WPARAM	wParam,
			LPARAM	lParam)
		{
			if (com::getLastError()) std::terminate();

			

			if (com::getLastError()) std::terminate();

			switch (message)						// Tells Windows We Want To Check The Message
			{
			case WM_CREATE:
				break;

			case WM_DESTROY:
			case WM_CLOSE:
				ChangeDisplaySettings(NULL, 0);

				wglMakeCurrent(hDC, NULL);
				wglDeleteContext(hRC);
				ReleaseDC(hWnd, hDC);

				PostQuitMessage(0);
				break;

			case WM_KEYDOWN:
				keys[wParam] = TRUE;
				break;

			case WM_KEYUP:
				keys[wParam] = FALSE;
				break;

			case WM_SIZE:
				//ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
				break;

			default:
				return (DefWindowProc(hWnd, message, wParam, lParam));
			}
			return (0);
		}

		bool binitialized = false;
		//GLFWwindow* window;
		vec2 res;

		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {
			MSG			msg;		// Windows Message Structure
			WNDCLASS	wc;			// Windows Class Structure Used To Set Up The Type Of Window
			HWND		hWnd;		// Storage For Window Handle

			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = (WNDPROC)WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;
			wc.lpszClassName = L"OpenGL WinClass";

			if (!RegisterClass(&wc))
			{
				MessageBox(0, L"Failed To Register The Window Class.", L"Error", MB_OK | MB_ICONERROR);
				return FALSE;
			}

			if (com::getLastError()) std::terminate();

			hWnd = CreateWindow(
				L"OpenGL WinClass",
				L"Jeff Molofee's GL Code Tutorial ... NeHe '99",		// Title Appearing At The Top Of The Window

				WS_POPUP |
				WS_CLIPCHILDREN |
				WS_CLIPSIBLINGS,

				0, 0,												// The Position Of The Window On The Screen
				640, 480,											// The Width And Height Of The WIndow

				NULL,
				NULL,
				hInstance,
				NULL);

			RECT	Screen;							// Used Later On To Get The Size Of The Window
			GLuint	PixelFormat;
			static	PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
				1,									// Version Number (?)
				PFD_DRAW_TO_WINDOW |				// Format Must Support Window
				PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,					// Must Support Double Buffering
				PFD_TYPE_RGBA,						// Request An RGBA Format
				24,									// Select A 16Bit Color Depth
				0, 0, 0, 0, 0, 0,					// Color Bits Ignored (?)
				0,									// No Alpha Buffer
				0,									// Shift Bit Ignored (?)
				0,									// No Accumulation Buffer
				0, 0, 0, 0,							// Accumulation Bits Ignored (?)
				32,									// 16Bit Z-Buffer (Depth Buffer)  
				0,									// No Stencil Buffer
				0,									// No Auxiliary Buffer (?)
				PFD_MAIN_PLANE,						// Main Drawing Layer
				0,									// Reserved (?)
				0, 0, 0								// Layer Masks Ignored (?)
			};

			if (com::getLastError()) std::terminate();

			hDC = GetDC(hWnd);				// Gets A Device Context For The Window

			if (com::getLastError()) std::terminate();

			PixelFormat = ChoosePixelFormat(hDC, &pfd);		// Finds The Closest Match To The Pixel Format We Set Above

			if (com::getLastError()) std::terminate();

			if (!PixelFormat)
			{
				MessageBox(0, L"Can't Find A Suitable PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);			// This Sends A 'Message' Telling The Program To Quit
				return NULL;						// Prevents The Rest Of The Code From Running
			}

			if (!SetPixelFormat(hDC, PixelFormat, &pfd))
			{
				MessageBox(0, L"Can't Set The PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);
				return NULL;
			}

			hRC = wglCreateContext(hDC);
			if (!hRC)
			{
				MessageBox(0, L"Can't Create A GL Rendering Context.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);
				return NULL;
			}

			if (!wglMakeCurrent(hDC, hRC))
			{
				MessageBox(0, L"Can't activate GLRC.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);
				return NULL;
			}

			GetClientRect(hWnd, &Screen);
			//InitGL(Screen.right, Screen.bottom);

			if (com::getLastError()) std::terminate();

			if (!hWnd)
			{
				MessageBox(0, L"Window Creation Error.", L"Error", MB_OK | MB_ICONERROR);
				return FALSE;
			}

			if (com::getLastError()) std::terminate();

			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
			SetFocus(hWnd);

			DEVMODE dmScreenSettings;											// Developer Mode

			memset(&dmScreenSettings, 0, sizeof(DEVMODE));						// Clear Room To Store Settings
			dmScreenSettings.dmSize = sizeof(DEVMODE);					// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth = 640;								// Screen Width
			dmScreenSettings.dmPelsHeight = 480;								// Screen Height
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;		// Pixel Mode
			ChangeDisplaySettings(&dmScreenSettings, NULL);			// Switch To Full Screen

			if (!wglMakeCurrent(hDC, hRC)) { deb::msberr("cant show make current"); std::terminate(); }

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
			SwapBuffers(hDC);
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
