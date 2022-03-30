#pragma once

// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>

#include "../parrlib/math/vector2f.h"
#include "../parrlib/math/vector4f.h"
#include "../parrlib/constants.h"

namespace dx11 {
	using namespace prb;

	namespace DXContext {
		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb);

		void init(HWND hWnd, int antiAlias, vec2 res);
		bool initialized();

		void setBlend(bool enabled);

		void clear(vec4 color);

		void present(bool vSync);

		void dispose();

		void setFullscreen(bool fullscreen);

		//resizing funcs
		void resize(vec2 bb);
		//void setRenderTarget(void* renderTarget);
		//void releaseBackBuffer();
		//void resizeSwapchainBuffers(vec2 size);
		//void createRenderTarget(void* renderTarget);
		//void setViewport(int topLeftX, int topLeftY, int width, int height);
	}

	extern "C" __declspec(dllexport) HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb);

	extern "C" __declspec(dllexport) void init(HWND hWnd, int antiAlias, vec2 res);
	extern "C" __declspec(dllexport) bool initialized();

	extern "C" __declspec(dllexport) void setBlend(bool enabled);

	extern "C" __declspec(dllexport) void clear(vec4 color);

	extern "C" __declspec(dllexport) void present(bool vSync);

	extern "C" __declspec(dllexport) void dispose();

	extern "C" __declspec(dllexport) void setFullscreen(bool fullscreen);

	extern "C" __declspec(dllexport) void resize(vec2 res);

	namespace prcdx = DXContext;

}
