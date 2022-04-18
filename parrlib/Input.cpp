#include "Input.h"

#include <parrlibcore/stringutils.h>
#include <parrlibcore/otherutil.h>
#include <parrlibcore/constants.h>

#include "debug.h"
#include "context.h"

namespace prb {
	namespace Input {
		const int MOUSE_NORMAL = 0;
		const int MOUSE_LOCKED = 1;
		const int MOUSE_HIDDEN = 2;

		int mouseStatus = MOUSE_NORMAL;

		HWND windowHwnd;

		const int KEYS_LENGTH = 349;
		const int MOUSE_LENGTH = 31;

		int mWheel = 0;

		bool oldkeys[KEYS_LENGTH];
		bool keys[KEYS_LENGTH];

		bool mkeys[MOUSE_LENGTH];
		bool oldmkeys[MOUSE_LENGTH];

		unsigned long long lastTextKey = 0;

		vec2 moldpos;
		vec2 mpos;
		vec2 mdelta;

		bool closing = false;
		RAWINPUTDEVICE Rid[1];
		std::thread rawInputThread;
		std::mutex rawInputMtx;
		HWND rawInputHwnd;
		vec2 accumMdelta = 0.f;

		LRESULT CALLBACK RawInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

			switch (message)
			{
			case WM_INPUT:
			{
				UINT dwSize;

				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
				    sizeof(RAWINPUTHEADER));
				LPBYTE lpb = new BYTE[dwSize];
				if (lpb == NULL)
				{
				    return DefWindowProc(hWnd, message, wParam, lParam);
				}

				if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
				    sizeof(RAWINPUTHEADER)) != dwSize)
				    OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

				RAWINPUT* raw = (RAWINPUT*)lpb;

				if (raw->header.dwType == RIM_TYPEKEYBOARD)
				{
				    
				}
				else if (raw->header.dwType == RIM_TYPEMOUSE)
				{
				    //deb::ss << raw->data.mouse.lLastX << " " << raw->data.mouse.lLastY << " " << raw->data.mouse.ulButtons << " " << raw->data.mouse.ulRawButtons << "\n";

				    //input::mdelta = vec2(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					std::lock_guard<std::mutex> lck(rawInputMtx);

					accumMdelta += vec2(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}

				delete[] lpb;
			}break;
			case WM_DESTROY:
			{
				closing = true;
				PostQuitMessage(0);
				return 0;
			}break;
			}

			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		void init(HWND hwnd) {
			windowHwnd = hwnd;
			for (int i = 0; i < KEYS_LENGTH; i++) {
				keys[i] = false;
			}

			for (int i = 0; i < MOUSE_LENGTH; i++) {
				mkeys[i] = false;
			}

			rawInputThread = std::thread([] {

				HINSTANCE hInstance = GetModuleHandle(NULL);

				WNDCLASSEX wc;
				ZeroMemory(&wc, sizeof(WNDCLASSEX));
				wc.cbSize = sizeof(WNDCLASSEX);
				wc.style = CS_HREDRAW | CS_VREDRAW;
				wc.lpfnWndProc = RawInputProc;
				wc.hInstance = hInstance;
				wc.lpszClassName = L"RawInputClass";
				RegisterClassEx(&wc);

				rawInputHwnd = CreateWindowEx(NULL,
					L"RawInputClass",
					L"RawInput",
					NULL, // WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					0, 0,
					1,
					1,
					NULL,
					NULL,
					hInstance,
					NULL
				);

				if(!UpdateWindow(rawInputHwnd))std::terminate();

				Rid[0].usUsagePage = 0x01;
				Rid[0].usUsage = 0x02;
				Rid[0].dwFlags = NULL;   // adds HID mouse and also ignores legacy mouse messages
				Rid[0].hwndTarget = rawInputHwnd;

				//Rid[1].usUsagePage = 0x01;
				//Rid[1].usUsage = 0x06;
				//Rid[1].dwFlags = NULL;   // adds HID keyboard and also ignores legacy keyboard messages
				//Rid[1].hwndTarget = rawInputHwnd;

				if (!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]))) std::terminate();

				Timer t;
				float awakeTimer = 0.f;

				MSG msg;
				while (!closing)
				{
					while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);

						if (msg.message == WM_QUIT) {
							closing = true;
							break;
						}
					}
					if (closing) break;

					if (!WaitMessage()) std::terminate();

					// WaitMessage ignores all the messages that arrive while this function's body is being
					// executed, which means that over time (if you always call WaitMessage), unread input
					// will eventually pile up permanently and cause a delay in mouse movements.
					// So everytime WaitMessage wakes the thread, i run it actively for like 100 milliseconds
					// so that every input is read and the queue (hopefully) emptied, then i recall WaitMessage
					// and reset the timer
					//t.set();
					//awakeTimer += t.getSec();
					//if (awakeTimer > .1f) { awakeTimer = 0.f; if (!WaitMessage()) std::terminate(); t.reset(); }
				}

			});

			//Rid[0].usUsagePage = 0x01;
			//Rid[0].usUsage = 0x02;
			//Rid[0].dwFlags = NULL;   // adds HID mouse and also ignores legacy mouse messages
			//Rid[0].hwndTarget = 0;

			//Rid[1].usUsagePage = 0x01;
			//Rid[1].usUsage = 0x06;
			//Rid[1].dwFlags = NULL;   // adds HID keyboard and also ignores legacy keyboard messages
			//Rid[1].hwndTarget = 0;

			//if (!RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) std::terminate();
		}

		void dispose() {
			closing = true;
			SendMessage(rawInputHwnd, WM_DESTROY, 0, 0);
			rawInputThread.join();
		}

		void update() {
			HWND foc = GetFocus();

			for (int i = 0; i < KEYS_LENGTH; i++) {
				oldkeys[i] = keys[i];
			}

			for (int i = 0; i < MOUSE_LENGTH; i++) oldmkeys[i] = mkeys[i];

			if (foc == windowHwnd && getClientRect().pointInside(getRawMousePos())) {
				for (int i = 0; i < 256; i++) {
					keys[i] = GetAsyncKeyState(i);
				}

				mkeys[0] = GetKeyState(VK_LBUTTON) < 0;
				mkeys[1] = GetKeyState(VK_RBUTTON) < 0;
			}
			else {
				for (int i = 0; i < KEYS_LENGTH; i++) if (keys[i]) keys[i] = false;
			}

			if (mouseStatus == MOUSE_LOCKED) {
				aabb2 crect = getClientRect().centered(); crect += crect.size() / 2.f; crect.scale(.5f);

				//deb::ss << crect << " " << mpos << "\n";

				//bool wrapped = false;

				//if (mpos.x > crect[2].x)			{ wrapped = true; setMousePos(vec2(crect[0].x+1.f, mpos.y)); }
				//else if (mpos.x < crect[0].x)		{ wrapped = true; setMousePos(vec2(crect[2].x-1.f, mpos.y)); }

				//if (mpos.y > crect[2].y)			{ /*deb::ss << "set " << crect[2].y << " " << mpos.y << " to " << vec2(mpos.x, crect[0].y) << "\n";*/ wrapped = true; setMousePos(vec2(mpos.x, crect[0].y+1.f)); }
				//else if (mpos.y < crect[0].y)		{ /*deb::ss << "set " << crect[0].y << " " << mpos.y << " to " << vec2(mpos.x, crect[2].y) << "\n";*/ wrapped = true; setMousePos(vec2(mpos.x, crect[2].y - 1.f)); }

				//if (wrapped) { mpos = getRawMousePos(); moldpos = mpos - mdelta; }
				//else { moldpos = mpos; mpos = getRawMousePos(); }

				//mdelta = mpos - moldpos;

				//mdelta = getRawMousePos() - (cst::res()/2.f).floored();
				//deb::ss << mdelta << " " << getRawMousePos() << " " << (cst::res() / 2.f).floored() << "\n";
				setMousePos((cst::res() / 2.f).floored());

				//deb::ss << "mouse centered " << getRawMousePos() << "\n";

				mpos = cst::res() / 2.f;
			}
			else {
				//moldpos = mpos;
				mpos = getRawMousePos();
				//mdelta = mpos - moldpos;
			}

			{
				std::lock_guard<std::mutex> lck(rawInputMtx);

				mdelta = accumMdelta;
				accumMdelta = 0.f;

				//if(mdelta != 0.f) deb::ss << mdelta << "\n";
			}

			//if (mouseStatus == MOUSE_LOCKED) {

			//	setMousePos(getWindowRect().center());

			//	mpos = cst::res() / 2.f;
			//	moldpos = mpos;
			//	mdelta = 0.f;
			//}
			//else {
			//}


		}

		bool getKey(int key) {
			return keys[key];
		}

		bool getKeyDown(int key) {
			return keys[key] && !oldkeys[key];
		}

		bool getKeyUp(int key) {
			return !keys[key] && oldkeys[key];
		}

		vec2 getRawMousePos() {
			POINT p;
			GetCursorPos(&p);

			RECT rec;
			GetWindowRect(windowHwnd, &rec);

			ScreenToClient(windowHwnd, &p);

			//RECT crec;
			//GetClientRect(windowHwnd, &crec);

			//return { (float)p.x - (float)rec.left - 8, (float)p.y - (float)rec.top - 31 };
			return { (float)p.x, (float)p.y };
		}

		vec2 getRawMouseDelta() {
			return mdelta;
		}

		vec2 getMousePos() {
			return (mpos / getClientRect().size() * 2.f - 1.f).ny();
		}

		vec2 getMouseDelta() {
			return mdelta / getClientRect().size() * 2.f;
		}

		vec2 getMouseDeltaConst() {
			//return mdelta / vec2(1920.f, 1080.f) * 60.f; //180.f/3.f, about 180deg in 20cm in a 1920x1080 screen with default windows mouse settings, feels right to me
			return mdelta / vec2(1080, 1080.f) * 0.3f; //180.f/3.f, about 180deg in 20cm in a 1920x1080 screen with default windows mouse settings, feels right to me
		}


		int getMWheel() { return mWheel; }

		bool getMouse(int button) { return button == 0 ? mkeys[0] : mkeys[1]; }
		bool getMouseDown(int button) { return mkeys[button] && !oldmkeys[button]; }
		bool getMouseUp(int button) { return !mkeys[button] && oldmkeys[button]; }

		void setMouseVisible(bool visible) { 
			if(visible) while (ShowCursor(true) < 0) {} 
			else		while (ShowCursor(false) >= 0) {} 
		}
		void showMouse() { setMouseVisible(true); }
		void hideMouse() { setMouseVisible(false); }

		void setMouseStatus(int status) {
			Input::mouseStatus = status;

			switch (status) {
				case MOUSE_NORMAL: showMouse(); break;
				case MOUSE_LOCKED: hideMouse(); break;
				case MOUSE_HIDDEN: hideMouse(); break;
			}

			if (status == MOUSE_LOCKED) {
				RECT r;
				GetWindowRect(windowHwnd, &r);
				ClipCursor(&r);
				//moldpos = (cst::res() / 2.f).floored();
				setMousePos((cst::res() / 2.f).floored());
			}
			else {
				ClipCursor(NULL);
			}
		}
		int getMouseStatus() { return mouseStatus; }

		void setMousePos(vec2 pos) { /*deb::ss << "set cursor pos from " << getRawMousePos();*/ POINT p = { (int)pos.x, (int)pos.y }; ClientToScreen(windowHwnd, &p); SetCursorPos(p.x, p.y); /*deb::ss << " to " << getRawMousePos() << " (target " << pos << ")\n";*/ }

		void toggleMouseStatus() { if (mouseStatus == MOUSE_NORMAL || mouseStatus == MOUSE_HIDDEN) setMouseStatus(MOUSE_LOCKED); else setMouseStatus(MOUSE_NORMAL); }


		vec2 getWindowPos() { return getWindowRect().fmin(); }
		aabb2 getClientRect() { RECT r; GetClientRect(windowHwnd, &r); return aabb2(vec2(r.left, r.bottom), vec2(r.right, r.top)); }
		aabb2 getWindowRect() { RECT r; GetWindowRect(windowHwnd, &r); return aabb2(vec2(r.left, r.bottom), vec2(r.right, r.top)); }


		bool leftClick() { return getMouseUp(0); }
		bool rightClick() { return getMouseUp(1); }
		bool click() { return getMouseUp(0); }

		bool leftClickDown() { return getMouseDown(0); }
		bool rightClickDown() { return getMouseDown(1); }
		bool clickDown() { return getMouseDown(0); }

		vec2 getIAxis2() {
			return vec2(
				getKey('D') - getKey('A'),
				getKey('W') - getKey('S')
			);
		}

		vec3 getIAxis3() {
			return vec3(
				getKey('D') - getKey('A'),
				getKey(VK_SPACE) - getKey('C'),
				getKey('W') - getKey('S')
			);
		}

		bool getAnyButtonMouseOrKeyboard() {
			for (int i = 1; i < KEYS_LENGTH; i++) if (keys[i])return true;
			for (int i = 1; i < MOUSE_LENGTH; i++) if (mkeys[i]) return true;
			if (mWheel != 0) return true;
			return false;
		}

		bool getAnyInputMouseOrKeyboard() {
			for (int i = 1; i < KEYS_LENGTH; i++) if (keys[i]) return true;
			for (int i = 1; i < MOUSE_LENGTH; i++) if (mkeys[i]) return true;
			if (mdelta.abs().magnitude() > 0.f) return true;
			if (mWheel != 0) return true;
			return false;
		}

		int returnAnyButtonMouseOrKeyboard() {
			for (int i = 1; i < KEYS_LENGTH; i++) if (keys[i]) return i;
			for (int i = 1; i < MOUSE_LENGTH; i++) if (mkeys[i]) return KEYS_LENGTH + i;
			if (mWheel != 0) return -2;
			return 0;
		}

		int returnAnyInputMouseOrKeyboard() {
			for (int i = 1; i < KEYS_LENGTH; i++) if (keys[i]) return i;
			for (int i = 1; i < MOUSE_LENGTH; i++) if (mkeys[i]) return KEYS_LENGTH + i;
			if (mdelta.abs().magnitude() > 0.f) return -1;
			if (mWheel != 0) return -2;
			return 0;
		}

		std::vector<int> getPressedKeys() {
			std::vector<int> keys;
			for (int i = 1; i < KEYS_LENGTH; i++) {
				if (getKey(i)) keys.push_back(i);

			}
			return keys;
		}

		std::vector<int> getPressedKeysDown() {
			std::vector<int> keys;
			for (int i = 1; i < KEYS_LENGTH; i++) {
				if (getKeyDown(i)) keys.push_back(i);

			}
			return keys;
		}

		std::vector<int> getPressedKeysUp() {
			std::vector<int> keys;
			for (int i = 1; i < KEYS_LENGTH; i++) {
				if (getKeyUp(i)) keys.push_back(i);

			}
			return keys;
		}

		unsigned long long pollTextKey() { unsigned long long tLastKey = lastTextKey; lastTextKey = 0; return tLastKey; }


		bool mouseInQuad(aabb2 const& bb) { return bb.pointInside(getMousePos()); }
		bool mouseInQuad(vec2 const& pos, vec2 const& size) { return mouseInQuad({ pos, pos + size }); }


		bool getCapsLockStatus() {
			return GetKeyState(VK_CAPITAL) & 0x0001 != 0;
		}

		void copyToClipboard(std::wstring text) {
			//HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1));
			//memcpy(GlobalLock(hMem), text.c_str(), (text.length() + 1));
			//GlobalUnlock(hMem);
			//OpenClipboard(0);
			//EmptyClipboard();
			//SetClipboardData(CF_UNICODETEXT, hMem);
			//CloseClipboard();
			if (OpenClipboard(0)) {
				EmptyClipboard();
				HGLOBAL hClipboardData;
				hClipboardData = GlobalAlloc(GMEM_DDESHARE, sizeof(wchar_t) * (wcslen(text.c_str()) + 1));

				WCHAR* pchData;
				pchData = (WCHAR*)GlobalLock(hClipboardData);
				wcscpy_s(pchData, (wcslen(text.c_str()) + 1), text.c_str());
				GlobalUnlock(hClipboardData);
				SetClipboardData(CF_UNICODETEXT, hClipboardData);
				CloseClipboard();
			}
		}

		std::wstring getFromClipboardw() {
			OpenClipboard(0);

			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			wchar_t* cText = static_cast<wchar_t*>(GlobalLock(hData));

			std::wstring clipboard = std::wstring(cText);

			GlobalUnlock(hData);

			CloseClipboard();

			return clipboard;
		}

		void copyToClipboard(std::string text) { copyToClipboard(stru::towstr(text)); }
		std::string getFromClipboard() { return stru::tostr(getFromClipboardw()); }
	}
}
