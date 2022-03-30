#pragma once

// include the basic windows header files and the Direct3D header files
#include <windows.h>

#include <functional>
#include <vector>
#include <chrono>
#include <filesystem>
#include <ShellScalingApi.h>

#include "../parrlib/math/vector2f.h"
#include "../parrlib/math/vector4f.h"
#include "../parrlib/constants.h"

#include "debug.h"

/*
	Main DirectX11 context
	funcs description:
	funcs[0 | FINIT] = init	--run once at the start
	funcs[1 | FUPDATE] = update --run every frame
	funcs[2 | FDRAW] = draw	--optional, run every frame after update()
	funcs[3 | FDESTROY] = destroy --run once when the application is closed (before DirectX11 destruction)
	funcs[4 | FRESIZE] = resize --run every frame while the app is being resized
	funcs[5 | FSTARTRESIZE] = startresize --run once when the app starts being resized
	funcs[6 | FENDRESIZE] = endresize --run once when the app stops being resized
	funcs[7 | FPREUPDATE] = preupdate --run every frame before starting the new frame (before clearing the current frame)
*/
namespace prb {
	namespace Context {
		HWND getPrimaryWindowIdentifier();

		extern const int FINIT;
		extern const int FUPDATE;
		extern const int FDRAW;
		extern const int FDESTROY;
		extern const int FRESIZE;
		extern const int FSTARTRESIZE;
		extern const int FENDRESIZE;
		extern const int FPREUPDATE;
		extern std::unordered_map<unsigned int, std::function<void()>> funcs;

		extern std::function<void(HWND, UINT, WPARAM, LPARAM)> fWindowProcPrec;
		extern std::function<void(HWND, UINT, WPARAM, LPARAM)> fWindowProcLate;

		extern std::unordered_map<std::string, std::string> fileSettings;

		void setRenderBackend(std::wstring const& backend);  //sets a render backend (OpenGL, DX11....), will append a ".dll" on windows systems
		std::wstring getRenderBackendW();

		void setRenderBackend(std::string const& backend);	 //sets a render backend (OpenGL, DX11....), will append a ".dll" on windows systems
		std::string getRenderBackend();

		void setPeekMessageAction(UINT val);
		UINT getPeekMessageAction();

		void setFinished(bool finished);
		bool getFinished();

		void setHandleMessageLoop(bool handleMessageLoop);
		bool getHandleMessageLoop();

		void setMaximized(bool maximized);
		bool getMaximized();

		void setWindowPosition(vec2 wPos);
		vec2 getWindowPosition();

		void setFullscreen(bool fullscreen);
		bool getFullscreen();

		void setTitle(std::wstring title);
		std::wstring getTitle();

		void setClearColor(vec4 color);
		vec4 getClearColor();

		double getCurTime();

		//skips the next frame, it is advised to call this function in FPREUPDATE
		void skipFrame();

		void setFramerateCap(int framerateCap);
		int getFramerateCap();

		int getMaxRefreshRate();

		bool isResizing();

		class displayDevice {
		public:
			std::wstring name;
			int index;

			class deviceMode {
			public:
				vec2 res; int refreshRate;
				DEVMODE rawMode;
				//DeviceMode(vec2 res, int refreshRate, DEVMODE rawMode) : res(res), refreshRate(refreshRate), rawMode(rawMode) {}
			};
			std::vector<deviceMode> vmodes;

			deviceMode getNative() {
				if (vmodes.size() == 0) return deviceMode();

				deviceMode res = vmodes[0];
				for (int i = 0; i < vmodes.size(); i++) {
					deb::tss << "iterating " << vmodes[i].res << " " << vmodes[i].refreshRate << " | " << res.res << " " << res.refreshRate << "\n"; deb::outStr();
					if (vmodes[i].res.x > res.res.x || vmodes[i].res.y > res.res.y || vmodes[i].refreshRate > res.refreshRate) {
						res = vmodes[i];
					}
				}
				return res;
			}
		};
		extern std::vector<displayDevice> dDevices;
		std::vector<displayDevice> getDisplayDevices();

		/*
			Initializes the main DirectX11 context and starts running the application
			See DXContext comments for funcs descriptions
		*/
		void setup(HINSTANCE hInstance);
		void setup(HINSTANCE hInstance, vec2 res);

		/*
			Initializes the main DirectX11 context and starts running the application
			funcs should be passed in this order (you can pass NULL or nullptr to skip functions you don't want to implement):
			init, update, draw, destroy, resize, startresize, endresize, preupdate
		*/
		void setup(HINSTANCE hInstance, std::vector<std::function<void()>> const& tfuncs);

		void setup(HINSTANCE hInstance, vec2 res, std::vector<std::function<void()>> const& tfuncs);

		void setVSync(bool vSync);
		bool getVSync();

		void setAntiAliasing(int aa);
		int getAntiAliasing();

		void resize(vec2 size);
		vec2 res();
	}

	namespace prc = Context;
}