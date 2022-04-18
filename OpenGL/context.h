#pragma once

#include <windows.h>

#include <glad/glad.h>

#include <parrlibcore/vector2f.h>
#include <parrlibcore/vector4f.h>

#include <parrlibcore/constants.h>

namespace gl {
	using namespace prb;
	namespace GLContext {
		HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb);

		void init(HWND hWnd, int antiAlias, vec2 res);
		bool initialized();

		void setBlend(bool enabled);

		void clear(vec4 color);

		void present(bool vSync);

		void dispose();

		void setFullscreen(bool fullscreen);

		void resize(vec2 bb);
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
}
