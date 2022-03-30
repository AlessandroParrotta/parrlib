#include "context.h"

namespace softst {
	namespace SoftwareSingleThreadContext {
		bool binitialized = false;

		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {
			int width = bb.sizex();
			int height = bb.sizey();
			RECT wr = { bb[0].x, bb[0].y, width, height };
			AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

			HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW,
				L"WindowClass",
				L"CPUGDITest",
				WS_OVERLAPPEDWINDOW, // | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				0, 0,
				wr.right - wr.left,
				wr.bottom - wr.top,
				NULL,
				NULL,
				hInstance,
				NULL
			);

			if (hWnd == NULL) return 0;
		}

		void init(HWND hWnd, int antiAlias, vec2 res) {

		}
		bool initialized() {
			return binitialized;
		}

		void setBlend(bool enabled) {

		}

		void clear(vec4 color) {

		}

		void present(bool vSync) {

		}

		void dispose() {

		}

		void setFullscreen(bool fullscreen) {

		}

		void resize(vec2 bb) {

		}
	}

	extern "C" __declspec(dllexport) HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {
		return SoftwareSingleThreadContext::createWindow(hInstance, title, bb);
	}

	extern "C" __declspec(dllexport) void init(HWND hWnd, int antiAlias, vec2 res) {
		SoftwareSingleThreadContext::init(hWnd, antiAlias, res);
	}
	extern "C" __declspec(dllexport) bool initialized() {
		return SoftwareSingleThreadContext::initialized();
	}

	extern "C" __declspec(dllexport) void setBlend(bool enabled) {
		SoftwareSingleThreadContext::setBlend(enabled);
	}

	extern "C" __declspec(dllexport) void clear(vec4 color){
		SoftwareSingleThreadContext::clear(color);
	}

	extern "C" __declspec(dllexport) void present(bool vSync) {
		SoftwareSingleThreadContext::present(vSync);
	}

	extern "C" __declspec(dllexport) void dispose() {
		SoftwareSingleThreadContext::dispose();
	}

	extern "C" __declspec(dllexport) void setFullscreen(bool fullscreen) {
		SoftwareSingleThreadContext::setFullscreen(fullscreen);
	}

	extern "C" __declspec(dllexport) void resize(vec2 res) {
		SoftwareSingleThreadContext::resize(res);
	}

}