#pragma once

#include <windows.h>
#include <functional>

#include <parrlibcore/vector2f.h>
#include <parrlibcore/vector4f.h>
#include <parrlibcore/utils2d/axisalignedboundingbox2d.h>

namespace prb {
	namespace impl {
		namespace context {
			typedef HWND(*CREATEWINDOW)(HINSTANCE hInstance, std::wstring title, aabb2 bb);		extern CREATEWINDOW createWindow;

			typedef void (*INIT)(HWND hWnd, int antiAlias, vec2 res);		extern INIT init;
			typedef bool (*INITIALIZED)();		extern INITIALIZED initialized;

			typedef void (*SETBLEND)(bool enabled);		extern SETBLEND setBlend;

			typedef void (*CLEAR)(vec4 color);		extern CLEAR clear;

			typedef void (*PRESENT)(bool vSync);		extern PRESENT present;

			typedef void (*DISPOSE)();		extern DISPOSE dispose;

			typedef void (*SETFULLSCREEN)(bool fullscreen);		extern SETFULLSCREEN setFullscreen;

			typedef void (*RESIZE)(vec2 res);		extern RESIZE resize;
		}
	}
}
