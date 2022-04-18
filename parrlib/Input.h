#pragma once

#include <windows.h>

#include <parrlibcore/vector2f.h>
#include <parrlibcore/vector3f.h>
#include <parrlibcore/utils2d/axisalignedboundingbox2d.h>

namespace prb {
	namespace DXVideoPlayer { class Sample3DSceneRenderer; }

	namespace Input {

		extern const int MOUSE_NORMAL;
		extern const int MOUSE_LOCKED;
		extern const int MOUSE_HIDDEN;

		extern unsigned long long lastTextKey;

		extern int mWheel;
		extern vec2 mdelta;

		extern RAWINPUTDEVICE Rid[1];

		void init(HWND window);
		void dispose();

		void update();

		bool getKey(int key);
		bool getKeyDown(int key);
		bool getKeyDown(int key);

		vec2 getRawMousePos();
		vec2 getRawMouseDelta();
		vec2 getMousePos();
		vec2 getMouseDelta();
		vec2 getMouseDeltaConst();

		int getMWheel();
		bool getMouse(int button);
		bool getMouseDown(int button);
		bool getMouseUp(int button);

		void setMouseVisible(bool visible);
		void showMouse();
		void hideMouse();
		
		void setMousePos(vec2 pos);

		void toggleMouseStatus();
		void setMouseStatus(int status);
		int getMouseStatus();


		vec2 getWindowPos();
		aabb2 getClientRect();
		aabb2 getWindowRect();


		bool leftClick();	//alias for input::getMouseUp(0)
		bool rightClick();	//alias for input::getMouseUp(1)
		bool click();		//alias for input::getMouseUp(0)

		bool leftClickDown();	//alias for input::getMouseDown(0)
		bool rightClickDown();	//alias for input::getMouseDown(1)
		bool clickDown();		//alias for input::getMouseDown(0)

		vec2 getIAxis2();
		vec3 getIAxis3();

		bool getAnyButtonMouseOrKeyboard();
		bool getAnyInputMouseOrKeyboard();

		std::vector<int> getPressedKeys();
		std::vector<int> getPressedKeysDown();
		std::vector<int> getPressedKeysUp();

		int returnAnyButtonMouseOrKeyboard();
		int returnAnyInputMouseOrKeyboard();

		unsigned long long pollTextKey();


		bool mouseInQuad(aabb2 const& bb);
		bool mouseInQuad(vec2 const& pos, vec2 const& size);

		aabb2 getClientRect();
		aabb2 getWindowRect();

		bool getCapsLockStatus();

		void copyToClipboard(std::wstring text);

		std::wstring getFromClipboardw();

		void copyToClipboard(std::string text);
		std::string getFromClipboard();
	}

#ifndef PARRLIBDX_INPUT_H_SHORT
#define PARRLIBDX_INPUT_H_SHORT
	namespace input = Input;
#endif
}
