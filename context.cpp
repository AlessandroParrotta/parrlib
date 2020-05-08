#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Context.h"

#include <iostream>
#include <chrono>
#include <time.h>
#include <thread>

#include "time.h"
#include "timer.h"
#include "input.h"
#include "debug.h"
#include "constants.h"
//#include "DebugMenu\DebugDisplay_Old.h"
#include "stringutils.h"

namespace Context {

	std::vector<std::string> args;

	std::string title = "OpenGL Parrlib Application";

	vec2 wndpos = 0.f;		//window position on the desktop
	vec2 wndsize = 0.f;		//window size on the desktop

	bool inApp = false;

	vec2 wsize = 0.f;		//frontbuffer size
	aabb2 wbb = 0.f;		//frontbuffer bounding box
	aabb2 sbb = 0.f;		//logical screen bounding box
	int refreshRate = 60;

	GLFWwindow* glfwwindow;
	GLFWmonitor* glfwmonitor;

	const GLFWvidmode* vidmodes;
	int vidmodecount = 0;
	
	const GLFWvidmode* monitorvidmode;

	const int FINIT = 0;
	const int FUPDATE = 1;
	const int FDRAW = 2;
	const int FDESTROY = 3;
	const int FRESIZE = 4;
	const int FSTARTRESIZE = 5;
	const int FENDRESIZE = 6;
	std::vector<std::function<void()>> funcs;

	bool fullscreen = false;
	bool vSync = false;
	bool focused = true;

	int framerateCap = 0;

	int oldMouseCursorDebug = GLFW_CURSOR_NORMAL;

	GLint fboid = 0;	//currently bound fbo
	FBO fbo;
	int scmode = 0;		//scaling mode, 0=onetoone, 1=intratio, 2=fratio, 3=fill

	vec4 clearColor = 1.f;
	void setClearColor(vec4 col) { clearColor = col; glClearColor(col.x, col.y, col.z, col.w); }
	vec4 getClearColor() { return clearColor; }

	void setFiltering(GLint filtering) { fbo.setFiltering(filtering); }

	Timer deltaTimer;
	double fps = 0.f;

	double lerpfps = fps;
	double lerpDTime = 0.f;

	//when glfwgetwindowmonitor is called, framebuffercallback is invoked and if left unchecked it'll 
	//cause an infinite recursive call, that's the reason for this variable
	bool changingFullscreen = false; 
	void setFullscreen(bool fullscreen) {
		if (prc::fullscreen == fullscreen || changingFullscreen)	return;
		changingFullscreen = true;
		prc::fullscreen = fullscreen;

		if (glfwGetWindowMonitor(glfwwindow)) {		//if it's fullscreen
			glfwSetWindowMonitor(glfwwindow, NULL,
				wndpos.x, wndpos.y,
				wndsize.x, wndsize.y,
				0);
		}
		else {										//if it's not fullscreen
			glfwmonitor = glfwGetPrimaryMonitor();
			if (glfwmonitor) {
				const GLFWvidmode* mode = glfwGetVideoMode(glfwmonitor);
				int px, py, wx, wy;
				glfwGetWindowPos(glfwwindow, &px, &py);
				glfwGetWindowSize(glfwwindow, &wx, &wy);
				glfwSetWindowMonitor(glfwwindow, glfwmonitor,
					0, 0, mode->width, mode->height,
					mode->refreshRate);
				wndpos = vec2(px, py);
				wndsize = vec2(wx, wy);
			}
		}

		setVSync(vSync);

		changingFullscreen = false;
	}

	void startresize() {
		funcs[FSTARTRESIZE]();
	}

	void endresize() {
		funcs[FENDRESIZE]();
	}

	bool dresizing = false;
	bool resizing = false;
	void resize(vec2 const& size) {
		if (size.x == 0 || size.y == 0) return;
		if (scmode == 0 && size != wsize) setscmode(2);

		resizing = true;
		if (!dresizing) { startresize(); }

		fbo.resize(size);
		funcs[FRESIZE]();
	}

	void setscmode(int sc) { if (scmode != 0 && sc == 0) resize(wsize); scmode = sc; }
	int getscmode() { return scmode; }

	void frame() {
		if (!resizing && dresizing) { endresize(); }
		dresizing = resizing;
		if (resizing) { resizing = false; }

		deltaTimer.set();
		gtime::deltaTime = deltaTimer.getSec();
		fps = 1.0 / deltaTimer.getSec();
		deltaTimer.reset();

		lerpfps += (fps - lerpfps) * 6.f * gtime::deltaTime;
		lerpDTime += (gtime::deltaTime - lerpDTime) * 6.f * gtime::deltaTime;
		debug::rtlog << "FPS: " << (int)lerpfps << " (" << stru::toWString(stru::getTimeFormattedSec(lerpDTime, 2)) << ")\n";

		gtime::time = glfwGetTime();

		input::processInput(glfwwindow);

		if (input::getKey(GLFW_KEY_LEFT_ALT) && input::getKeyDown(GLFW_KEY_ENTER)) setFullscreen(!fullscreen);
		if (input::getKeyDown(GLFW_KEY_F1)) {
			debugmenu::enabled = !debugmenu::enabled; 

			if (debugmenu::enabled) {
				oldMouseCursorDebug = input::getCursorInputMode();
				if (input::getCursorInputMode() != GLFW_CURSOR_NORMAL) input::toggleCursorInputMode();
			}
			else {
				input::setCursorInputMode(oldMouseCursorDebug);
			}
		}
		//if (input::getKeyDown(GLFW_KEY_F3)) setVSync(!vSync);
		//if (input::getKeyDown(GLFW_KEY_F4)) debug::enabled = !debug::enabled;
		//if (input::getKey(GLFW_KEY_F1)) resize((input::getRawMousePos().invy(wsize.y) - wsize / 2.f).abs()*2.f);
		//if (input::getKeyDown(GLFW_KEY_F5)) { setscmode((scmode + 1) % 4); }

		//reset the matrix status in case of modifications in update()
		glLoadIdentity();
		glViewport(0, 0, wsize.x, wsize.y);
		glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		util::texEnable();
		util::blendEnable();

		cst::res(wsize);

		switch (scmode) {
		case 0: case 3: sbb = { 0.f, wsize }; sbb = pmat3::getNdc(wbb) * sbb; break;
			case 1: 
			{
				vec2 perc = wbb.size() / fbo.size();

				float scale = std::fmax(perc.minv(), 0.f);
				if (scale < 1.f) scale = 1.f / std::ceilf(1.f / scale);
				else scale = std::floorf(scale);

				sbb = (aabb2{ 0.f, fbo.size() } * scale).centered() + wsize/2;
				sbb = pmat3::getNdc(wbb) * sbb;
			}
			break;	
			case 2: sbb = { 0.f, fbo.size() }; sbb = pmat3::getNdc(wbb) * sbb.fitted(wbb); break;
		}

		if (fbo.size() != wsize) {
			fbo.clear(clearColor);
			fbo.bind();
		}
		else { glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

			util::multMatrix(util::getAspectOrtho());

			inApp = true;

			funcs[FUPDATE]();
			funcs[FDRAW]();

			if(!debugmenu::enabled) imui::reset();

			inApp = false;

			if (util::mStack.size() > 1) std::cout << "warning: degenerate matrix stack at end of frame\n";
			util::mStack.clear();
			util::mStack.push_back(1.f);

			util::multMatrix(util::getAspectOrtho());

			util::setColor(vc4::white);
			debug::drawObjects();

			util::resetMatrix();

		if (fbo.size() != wsize) {
			fbo.unbind();
		}

		glUseProgram(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);


		util::setColor(vc4::white);

		if (fbo.size() != wsize) {
			util::drawTexture(fbo.getAttachment(GL_COLOR_ATTACHMENT0), sbb, 1.f);
		}

		util::blendEnable();
		util::texEnable();

		cst::res(wsize);

		util::multMatrix(util::getAspectOrtho());

		imui::setSpace(1.f);
		if (debugmenu::enabled) { debugmenu::update(); imui::reset();}

		debug::draw();
		debug::update();

		input::update();

		util::resetMatrix();

		glfwSwapBuffers(glfwwindow);
		glfwPollEvents();

		util::checkForGLErrors();
	}

	std::chrono::high_resolution_clock::time_point microsNow;
	std::chrono::high_resolution_clock::time_point microsLater;
	void start() {
		while (!glfwWindowShouldClose(glfwwindow)) {

			if (framerateCap >= 5)
				microsNow = std::chrono::high_resolution_clock::now();

			frame();

			if (framerateCap >= 5) {
				microsLater = std::chrono::high_resolution_clock::now();
				double microsToWait = (double)1e06 / (double)framerateCap;

				std::chrono::duration<uint64_t, std::nano> timeDiff = microsLater - microsNow;
				double renderTime = timeDiff.count() / (double)1e03;


				if (microsToWait > renderTime) {
					double timeToSleep = microsToWait - renderTime;
					double timeRounded = (int)((floor(timeToSleep / 1000.) - 1.) * 1000.);

					Timer waitT;
					if (timeRounded > 0.) std::this_thread::sleep_for(std::chrono::microseconds((int)timeRounded));
					waitT.set();

					double sleepTime = waitT.getMicro();
					if (sleepTime < timeToSleep) {
						while (sleepTime < timeToSleep) {
							waitT.set();
							sleepTime = waitT.getMicro();
						}
					}
				}
			}

			if (!focused) {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}
	}

	void curposcallback(GLFWwindow* w, double xpos, double ypos) {
		input::mousePos = input::rawMousePos = vec2(xpos, ypos);
	}

	void framebuffersizecallback(GLFWwindow* w, int width, int height) {
		glViewport(0, 0, width, height);
		wsize = vec2(width, height);
		wbb = { 0.f, wsize };

		if (scmode == 0) { resize(vec2(width, height)); }
		frame();
	}

	void scrollcallback(GLFWwindow* window, double xoffset, double yoffset) {
		input::scrollWheel = (int)yoffset;
	}

	void charcallback(GLFWwindow* window, unsigned int codepoint) {
		input::textKeys.push_back(codepoint);
	}

	//hide the console in int main()
	//HWND hWnd = GetConsoleWindow();
	//ShowWindow(hWnd, SW_HIDE);
	void setup(std::vector<std::function<void()>> const& tfuncs, std::string const& args) {
		Timer tinit;
		std::cout << "initializing...\n";

		funcs = tfuncs;
		if (funcs.size() < 7) {
			while (funcs.size() < 7) funcs.push_back([] {});
		}
		for (int i = 0; i < funcs.size(); i++) { if (funcs[i] == NULL) funcs[i] = [] {}; }

		if (!glfwInit()) { std::cout << "failed to initialize GLFW!\n"; std::terminate(); }

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		glfwmonitor = glfwGetPrimaryMonitor();
		if (!glfwmonitor) { std::cout << "error trying to get primary monitor\n"; std::terminate(); }
		
		monitorvidmode = glfwGetVideoMode(glfwmonitor);
		if (!monitorvidmode) { std::cout << "error trying to get video mode\n"; std::terminate(); }

		refreshRate = monitorvidmode->refreshRate;
		cst::res(vec2(monitorvidmode->width, monitorvidmode->height));

		glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
		
		std::cout << "native config: " << monitorvidmode->width << "x" << monitorvidmode->height << " @ " << monitorvidmode->refreshRate << "hz (bits " << monitorvidmode->redBits << "r" << monitorvidmode->greenBits << "g" << monitorvidmode->blueBits << "b)\n";

		vidmodes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &vidmodecount);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

		glfwwindow = glfwCreateWindow(monitorvidmode->width, monitorvidmode->height, title.c_str(), NULL, NULL);
		if (!glfwwindow) { std::cout << "failed to create window!\n"; std::terminate(); }

		glfwMakeContextCurrent(glfwwindow);
		glfwSetCursorPosCallback(glfwwindow, curposcallback);
		glfwSetFramebufferSizeCallback(glfwwindow, framebuffersizecallback);
		glfwSetScrollCallback(glfwwindow, scrollcallback);
		glfwSetCharCallback(glfwwindow, charcallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ std::cout << "Failed to initialize GLAD" << std::endl; std::terminate(); }
	
		wsize = vec2(monitorvidmode->width, monitorvidmode->height);
		cst::res(wsize);

		input::addActiveLayer(INPUT_LAYER_DEFAULT);

		input::init();
		debug::init();
		//DebugDisplay_Old::init();

		setVSync(vSync);

		fbo = FrameBufferObject((int)cst::resx(), (int)cst::resy(), GL_NEAREST, GL_NEAREST);
		fbo.setFiltering(GL_NEAREST, GL_NEAREST);

		tinit.set(); std::cout << "initialized (" << tinit.getTime() << ")\n";

		imui::init();
		imui::useAtlas("assets/sprites/atlas.png");
		imui::setTxr("assets/fonts/segoeui.ttf", 32);
		imui::setTextColor(vc4::black);

		debugmenu::init();

		funcs[FINIT]();

		glfwMaximizeWindow(glfwwindow);

		start();

		funcs[FDESTROY]();

		debugmenu::dispose();

		glfwTerminate();
	}
	
	void setup(std::vector<std::function<void()>> const& funcs) { setup(funcs, {}); }

	void setVSync(bool vSync) {
		glfwSwapInterval(vSync ? 1 : 0);

		prc::vSync = vSync;
	} 
	bool getVSync() { return vSync; }

	vec2 wres() { return wsize; }
	vec2 res() { return fbo.size(); }

	std::unordered_map<std::string, Shader> gshaders;	//global shaders
	void setShader(std::string const& name, Shader const& shader) { gshaders[name] = shader; }
	Shader& getShader(std::string const& name) { return gshaders[name]; }

	std::unordered_map<std::string, VAO> gvaos;			//global vbos
	void setvao(std::string const& name, VAO const& vao) { gvaos[name] = vao; }
	VAO& getvao(std::string const& name) { return gvaos[name]; }
}

//void window_iconify_callback(GLFWwindow* window, int iconified) {
//	std::cout << "Iconify callback!\n";
//}
//
//void window_refresh_callback(GLFWwindow* window) {
//
//}
//
//void window_focus_callback(GLFWwindow* window, int focused) {
//	std::cout << "Focus callback!\n";
//}
