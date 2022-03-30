#include "context.h"

#include "math/timer.h"

#include "Input.h"
#include "debug.h"
#include "shader.h"
#include "util.h"
#include "ui/imui.h"
#include "debugmenu/debugmenu.h"

namespace prb {
    namespace Context {
        HWND windowHwnd;
        HWND getPrimaryWindowIdentifier() { return windowHwnd; }

        const int F_SIZE = 8;

        const int FINIT = 0;
        const int FUPDATE = 1;
        const int FDRAW = 2;
        const int FDESTROY = 3;
        const int FRESIZE = 4;
        const int FSTARTRESIZE = 5;
        const int FENDRESIZE = 6;
        const int FPREUPDATE = 7;
        std::unordered_map<unsigned int, std::function<void()>> funcs;

        std::function<void(HWND, UINT, WPARAM, LPARAM)> fWindowProcPrec = [](HWND, UINT, WPARAM, LPARAM) {};
        std::function<void(HWND, UINT, WPARAM, LPARAM)> fWindowProcLate = [](HWND, UINT, WPARAM, LPARAM) {};
        
        std::unordered_map<std::string, std::string> fileSettings;

        std::wstring renderBackend = L"none";
        void setRenderBackend(std::wstring const& backend) { prc::renderBackend = backend; }
        std::wstring getRenderBackendW() { return renderBackend; }

        void setRenderBackend(std::string const& backend) { setRenderBackend(stru::towstr(backend)); }
        std::string getRenderBackend() { return stru::tostr(getRenderBackend()); }

        void loadAndApplySettings() {

            std::string settingsFilePath = outl::getExeFolder() + "/settings.cfg";
            if (std::filesystem::exists(settingsFilePath)) {
                std::ifstream f(settingsFilePath);

                if (f.good()) {
                    std::string line;
                    while (std::getline(f, line)) {
                        deb::pr(line.c_str(), "\n");

                        std::stringstream ss; ss << line;
                        std::string key; ss >> key;
                        std::string value; int skipSpaces = ss.tellg(); while (line[skipSpaces] == ' ' && skipSpaces < line.length()) skipSpaces++; for (int i = skipSpaces; i < line.length(); i++) value += line[i];

                        fileSettings[key] = value;
                        deb::pr(key.c_str(), " ", fileSettings[key].c_str(), "\n");
                    }
                }
                else deb::pr("error reading \"settings.cfg\"!\n");

                f.close();
            }
            else deb::pr("no \"settings.cfg\" found!\n");

            if (renderBackend.compare(L"none") == 0)
                if (fileSettings.find("backend") != fileSettings.end()) renderBackend = stru::towstr(fileSettings["backend"]);
                else                                                    renderBackend = L"DX11"; //TODO system to figure out best backend available

            if (fileSettings.find("pos") != fileSettings.end()) {
                std::stringstream ss; ss << fileSettings["pos"];
                setWindowPosition(v2u::get(ss));
            }

            if(fileSettings.find("res") != fileSettings.end()){
                std::stringstream ss; ss << fileSettings["res"];
                cst::res(v2u::get(ss));
            }

            if (fileSettings.find("vsync") != fileSettings.end()) {
                std::stringstream ss; ss << fileSettings["vsync"];
                setVSync(stru::geti(ss));
            }
        }

        UINT peekMessageAction = PM_REMOVE;
        void setPeekMessageAction(UINT val) { prc::peekMessageAction = val; }
        UINT getPeekMessageAction() { return peekMessageAction; }

        std::vector<displayDevice> dDevices;
        std::vector<displayDevice> getDisplayDevices() {
            std::vector<displayDevice> res;

            //get active display devices (monitors)
            DWORD DispNum = 0;
            DISPLAY_DEVICE  DisplayDevice;
            DEVMODE defaultMode;

            // initialize DisplayDevice
            ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
            DisplayDevice.cb = sizeof(DisplayDevice);

            while (EnumDisplayDevices(NULL, DispNum, &DisplayDevice, 0)) {
                ZeroMemory(&defaultMode, sizeof(DEVMODE));
                defaultMode.dmSize = sizeof(DEVMODE);

                displayDevice dd;

                //deb::ss << "display device " << DisplayDevice.DeviceName << " (" << DispNum << "):\n";

                int iModeNum = 0;
                while (EnumDisplaySettings((LPWSTR)DisplayDevice.DeviceName, iModeNum, &defaultMode)) {
                    if (defaultMode.dmPelsWidth > 0 && defaultMode.dmPelsHeight > 0) {
                        //deb::ss << "\t" << defaultMode.dmPelsWidth << "x" << defaultMode.dmPelsHeight << "@" << defaultMode.dmDisplayFrequency << "Hz (" << defaultMode.dmDeviceName << ")" << "\n";
                        dd.vmodes.push_back(displayDevice::deviceMode{ vec2(defaultMode.dmPelsWidth, defaultMode.dmPelsHeight), (int)defaultMode.dmDisplayFrequency, defaultMode });
                    }
                    iModeNum++;
                }

                dd.name = DisplayDevice.DeviceName;
                dd.index = iModeNum;

                std::reverse(dd.vmodes.begin(), dd.vmodes.end());

                res.push_back(dd);

                ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
                DisplayDevice.cb = sizeof(DisplayDevice);
                DispNum++;
            }

            //std::reverse(res.begin(), res.end());

            return res;
        }

        vec2 wPos = 0.f;
        void setWindowPosition(vec2 wPos) { prc::wPos = wPos; }
        vec2 getWindowPosition() { return wPos; }

        bool fullscreen = false;
        void setFullscreen(bool fullscreen) { Context::fullscreen = fullscreen; impl::context::setFullscreen(fullscreen); }
        bool getFullscreen() { return fullscreen; }

        bool maximized = false;
        void setMaximized(bool maximized) { prc::maximized = maximized; }
        bool getMaximized() { return maximized; }

        bool finished = false;
        void setFinished(bool finished) { prc::finished = finished; }
        bool getFinished() { return finished; }

        bool handleMessageLoop = true;
        void setHandleMessageLoop(bool handleMessageLoop) { prc::handleMessageLoop = handleMessageLoop; }
        bool getHandleMessageLoop() { return handleMessageLoop; }

        double getCurTime() { return gtime::time; }

        bool initialized = false;

        std::wstring title = L"Parrlib Application";
        void setTitle(std::wstring title) {
            prc::title = title;
            if (initialized) {
                SetWindowTextW(windowHwnd, title.c_str());
            }
        }
        std::wstring getTitle() { return title; }

        bool vSync = false;
        bool focused = true;

        int framerateCap = 0;
        void setFramerateCap(int framerateCap) { prc::framerateCap = framerateCap; }
        int getFramerateCap() { return framerateCap; }

        std::chrono::high_resolution_clock::time_point microsNow;
        std::chrono::high_resolution_clock::time_point microsLater;

        std::chrono::high_resolution_clock::time_point deltaNow;
        std::chrono::high_resolution_clock::time_point deltaLater;

        vec4 clearColor = "#000000ff";
        void setClearColor(vec4 color) { clearColor = color; }
        vec4 getClearColor() { return clearColor; }

        void setVSync(bool vSync) { prc::vSync = vSync; }
        bool getVSync() { return vSync; }

        int antiAlias = 1;
        void setAntiAliasing(int aa) { prc::antiAlias = aa; }
        int getAntiAliasing() { return antiAlias; }

        bool skipFrameb = false;
        void skipFrame() { skipFrameb = true; }

        int maxRefreshRate = 0;
        int getMaxRefreshRate() { return maxRefreshRate; }

        bool oldResizing = false, resizing = false;
        bool isResizing() { return resizing; }

        int oldMouseCursorDebug;

        void RenderFrame(void)
        {
            if (resizing && !oldResizing)   funcs[FSTARTRESIZE]();
            if (resizing)                   funcs[FRESIZE]();
            if (!resizing && oldResizing)   funcs[FENDRESIZE]();

            deltaLater = std::chrono::high_resolution_clock::now();
            std::chrono::duration<uint64_t, std::nano> timeDiff = deltaLater - deltaNow;
            double renderTime = timeDiff.count();

            gtime::deltaTime = renderTime / (double)1e09;

            deltaNow = std::chrono::high_resolution_clock::now();

            gtime::time += gtime::deltaTime;

            deb::rtss << "FPS: " << (1. / gtime::deltaTime) << "\n";

            impl::context::clear(clearColor);
            impl::context::setBlend(true);

            funcs[FUPDATE]();
            funcs[FDRAW]();

            if (!debugmenu::enabled) imui::reset();

            if (input::getKeyDown(VK_F1)) {
                debugmenu::enabled = !debugmenu::enabled;

                if (debugmenu::enabled) {
                    oldMouseCursorDebug = input::getMouseStatus();
                    if (input::getMouseStatus() != input::MOUSE_NORMAL) input::toggleMouseStatus();
                }
                else {
                    input::setMouseStatus(oldMouseCursorDebug);
                }
            }
            if (debugmenu::enabled) { debugmenu::update(); imui::reset(); }

            deb::drawDebStrs(cst::res());

            impl::context::present(vSync);

            if (util::mStack.size() > 1) {
                //deb::ss << "warning: degenerate matrix stack\n";
                util::mStack.clear(); util::mStack.push_back(1.f);
            }

            Input::mWheel = 0;

            oldResizing = resizing;
            resizing = false;

            imui::reset();
        }

        void InitGraphics()
        {

        }

        void InitPipeline()
        {
            //defShader = { "default" };
            //defShader.use();

            //defTexShader = { "texture" };
            util::init();
        }

        void Init(HWND hWnd)
        {
            impl::context::init(hWnd, antiAlias, cst::res());
        }

        void Clean(void)
        {
            funcs[FDESTROY]();

            impl::context::dispose();
        }

        LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            fWindowProcPrec(hWnd, message, wParam, lParam);

            switch (message)
            {
            case WM_CREATE:
            {
                PAINTSTRUCT ps;
                HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));

                HDC hdc = BeginPaint(hWnd, &ps);
                    FillRect(hdc, &ps.rcPaint, br);
                EndPaint(hWnd, &ps);

                DeleteObject(br);
            }break;
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
            case WM_WINDOWPOSCHANGED:
            {
                HMONITOR mntr = MonitorFromWindow(windowHwnd, MONITOR_DEFAULTTONEAREST);

                DEVICE_SCALE_FACTOR dsf;
                GetScaleFactorForMonitor(mntr, &dsf);

                deb::pr("dsf", outl::monitorScalingToPerc(dsf), "\n");
                cst::scalingFactor(outl::monitorScalingToPerc(dsf));
            }break;
            case WM_SIZE:
            {
                if (!impl::context::initialized()) return 0;
                //if (!swapchain) return 0;
                //RECT rect; GetWindowRect(hWnd, &rect);
                RECT rect; GetClientRect(hWnd, &rect);
                int nSizeX = rect.right - rect.left, nSizeY = rect.bottom - rect.top;

                impl::context::resize(vec2(nSizeX, nSizeY) * cst::scalingFactor());

                AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

                cst::res(vec2(nSizeX, nSizeY) * cst::scalingFactor());

                resizing = true;

                deb::pr(cst::res(), "\n");

                funcs[FSTARTRESIZE]();
                funcs[FRESIZE]();  
                funcs[FENDRESIZE]();

                RenderFrame();
            }break;
            case WM_MOUSEWHEEL:
            {
                short wval = (short)(wParam >> 16);
                Input::mWheel = (wval > 0 ? 1 : (wval < 0 ? -1 : 0));
            }break;
            case WM_INPUT:
            {
                //UINT dwSize;

                //GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
                //    sizeof(RAWINPUTHEADER));
                //LPBYTE lpb = new BYTE[dwSize];
                //if (lpb == NULL)
                //{
                //    return 0;
                //}

                //if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
                //    sizeof(RAWINPUTHEADER)) != dwSize)
                //    OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

                //RAWINPUT* raw = (RAWINPUT*)lpb;

                //if (raw->header.dwType == RIM_TYPEKEYBOARD)
                //{
                //    //hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
                //    //    raw->data.keyboard.MakeCode,
                //    //    raw->data.keyboard.Flags,
                //    //    raw->data.keyboard.Reserved,
                //    //    raw->data.keyboard.ExtraInformation,
                //    //    raw->data.keyboard.Message,
                //    //    raw->data.keyboard.VKey);
                //    //if (FAILED(hResult))
                //    //{
                //    //    // TODO: write error handler
                //    //}
                //    //OutputDebugString(szTempOutput);
                //}
                //else if (raw->header.dwType == RIM_TYPEMOUSE)
                //{
                //    //hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
                //    //    raw->data.mouse.usFlags,
                //    //    raw->data.mouse.ulButtons,
                //    //    raw->data.mouse.usButtonFlags,
                //    //    raw->data.mouse.usButtonData,
                //    //    raw->data.mouse.ulRawButtons,
                //    //    raw->data.mouse.lLastX,
                //    //    raw->data.mouse.lLastY,
                //    //    raw->data.mouse.ulExtraInformation);

                //    //if (FAILED(hResult))
                //    //{
                //    //    // TODO: write error handler
                //    //}
                //    //OutputDebugString(szTempOutput);

                //    deb::ss << raw->data.mouse.lLastX << " " << raw->data.mouse.lLastY << " " << raw->data.mouse.ulButtons << " " << raw->data.mouse.ulRawButtons << "\n";

                //    input::mdelta = vec2(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                //}

                //delete[] lpb;
            }break;
            case WM_PAINT:
            {
                if (gtime::time < .1f) {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hWnd, &ps);
                    FillRect(hdc, &ps.rcPaint, (HBRUSH)(2)); //2 is black, for some reason
                    EndPaint(hWnd, &ps);

                    //to draw on the frame outside the client area you need a Window Device Context (use GetDCEx with DCX_WINDOW as flags probably)
                    //https://docs.microsoft.com/en-us/windows/win32/gdi/display-device-contexts
                }
                break;
            }
            case WM_CHAR:
            {
                input::lastTextKey = wParam;
            }break;
            case WM_GETDPISCALEDSIZE:
            {
                //deb::pr("dpi changed\n");
            }break;
            default:
            {
                //if(
                //    message != 512 &&
                //    message != 32 && 
                //    message != 132 && 
                //    message != 534 && 
                //    message != 70 && 
                //    message != 71 && 
                //    message != 8 && 
                //    message != 634 && 
                //    message != 642 && 
                //    message != 3 && 
                //    message != 36 && 
                //    message != 674 && 
                //    message != 533 && 
                //    message != 562 && 
                //    message != 561 && 
                //    message != 274 && 
                //    message != 20 && 
                //    message != 133 && 
                //    message != 160
                //)
                //    deb::pr("unknown window message received: ", message, "\n");
            }break;
            }

            //deb::pr(GetDpiForWindow(windowHwnd), "\n");
            //GetScaleFactorForMonitor

            fWindowProcLate(hWnd, message, wParam, lParam);

            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        void setup(HINSTANCE hInstance) {
            for (int i = 0; i < F_SIZE; i++) if (funcs.find(i) == funcs.end()) funcs[i] = [] {};

            loadAndApplySettings();

            //impl::init(L"DX11.dll");
            impl::init(renderBackend + L".dll");

            deb::createWindow();

            dDevices = getDisplayDevices();

            const HWND hDesk = GetDesktopWindow();
            RECT desktop; GetWindowRect(hDesk, &desktop);
            if (cst::res() <= 0.f) cst::res(vec2(desktop.right, desktop.bottom));

            //deb::debss << "cst res pre " << cst::res(); deb::msbinfo();

            WNDCLASSEX wc;

            ZeroMemory(&wc, sizeof(WNDCLASSEX));

            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = WindowProc;
            wc.hInstance = hInstance;
            //wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
            //wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.lpszClassName = L"WindowClass";

            RegisterClassEx(&wc);

            RECT wr = { 0, 0, (int)cst::resx(), (int)cst::resy() };
            AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

            //windowHwnd = CreateWindowEx(NULL,
            //    L"WindowClass",
            //    title.c_str(),
            //    WS_OVERLAPPEDWINDOW,
            //    wPos.x,
            //    wPos.y,
            //    wr.right - wr.left,
            //    wr.bottom - wr.top,
            //    NULL,
            //    NULL,
            //    hInstance,
            //    NULL
            //);

            //windowHwnd = impl::context::createWindow(hInstance, title, { {wPos.x, wPos.y}, vec2(wr.right - wr.left, wr.bottom - wr.top) });
            windowHwnd = impl::context::createWindow(hInstance, title, { wPos, cst::res() });
            if (!windowHwnd) std::terminate();

            //THIS IS A GLOBAL COLOR CHANGE, EVEN OTHER PROGRAMS
            //int aElements[2] = { COLOR_WINDOW, COLOR_ACTIVECAPTION };
            //DWORD aNewColors[]{RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF)};
            //SetSysColors(2, aElements, aNewColors);

            ShowWindow(windowHwnd, maximized ? SW_MAXIMIZE : SW_NORMAL);
            UpdateWindow(windowHwnd);

            RECT rect; GetClientRect(windowHwnd, &rect);
            int nSizeX = rect.right - rect.left, nSizeY = rect.bottom - rect.top;
            cst::res(vec2(nSizeX, nSizeY));

            //deb::debss << cst::res(); deb::msbinfo();

            Init(windowHwnd);

            impl::context::resize(cst::res());

            util::init();

            Input::init(windowHwnd);

            imui::init();
            imui::useAtlas("sprites/atlas.png");
            imui::setTxr("segoeui.ttf", 32);
            imui::setTextColor(vc4::white);

            debugmenu::init();

            deb::init();

            deltaNow = std::chrono::high_resolution_clock::now();

            if (dDevices.size() == 0) { deb::mbe("No display device found!"); return; }

            maxRefreshRate = 0;
            for (int i = 0; i < dDevices[0].vmodes.size(); i++) {
                //deb << "iterating " << vmodes[i].refreshRate << "\n";
                maxRefreshRate = (std::max)(maxRefreshRate, dDevices[0].vmodes[i].refreshRate);
            }
            //deb << "maxref " << maxRefreshRate << "\n";
            //framerateCap = maxRefreshRate;

            initialized = true;

            funcs[FINIT]();

            int timesRendered = 0;

            //timeBeginPeriod(1);

            MSG msg;
            while (!finished)
            {
                if (handleMessageLoop) {
                    while (PeekMessage(&msg, NULL, 0, 0, peekMessageAction))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);

                        if (msg.message == WM_QUIT) {
                            finished = true;
                            break;
                        }
                    }
                    if (finished) break;
                }

                Input::update();

                funcs[FPREUPDATE]();
                //if (skipFrameb) { skipFrameb = false; return; }

                if (skipFrameb) {
                    deltaLater = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<uint64_t, std::nano> timeDiff = deltaLater - deltaNow;
                    double renderTime = timeDiff.count();

                    gtime::deltaTime = renderTime / (double)1e09;

                    deltaNow = std::chrono::high_resolution_clock::now();

                    gtime::time += gtime::deltaTime;
                }

                if (!skipFrameb) {
                    if (input::getKey(VK_MENU) && input::getKeyDown(VK_RETURN)) {
                        setFullscreen(!getFullscreen());
                    }

                    if (framerateCap > 5)
                        microsNow = std::chrono::high_resolution_clock::now();

                    RenderFrame();

                    if (framerateCap > 5) {
                        microsLater = std::chrono::high_resolution_clock::now();
                        double microsToWait = (double)1e06 / (double)framerateCap;

                        std::chrono::duration<uint64_t, std::nano> timeDiff = microsLater - microsNow;
                        double renderTime = timeDiff.count() / (double)1e03;


                        if (microsToWait > renderTime) {
                            double timeToSleep = microsToWait - renderTime;
                            double timeRounded = (int)((floor(timeToSleep / 1000.) - 1.) * 1000.);

                            //deb::rtss << "sleep for " << timeRounded/1000. << "ms\n";

                            Timer waitT;
                            if (timeRounded > 0.) std::this_thread::sleep_for(std::chrono::microseconds((int)timeRounded));
                            waitT.set();

                            //Timer busyT;

                            double sleepTime = waitT.getMicro();
                            if (sleepTime < timeToSleep) {
                                while (sleepTime < timeToSleep) {
                                    waitT.set();
                                    sleepTime = waitT.getMicro();
                                }
                            }

                            //busyT.set();
                            //deb::rtss << "slept busy " << busyT.getTime().c_str() << "\n";
                        }
                    }
                }

                skipFrameb = false;
            }

            debugmenu::dispose();
            deb::dispose();
            input::dispose();

            Clean();

            //timeEndPeriod(1);
        }

        void setup(HINSTANCE hInstance, std::vector<std::function<void()>> const& tfuncs) {
            //funcs = tfuncs;
            //if (funcs.size() < 8) {
            //	while (funcs.size() < 8) funcs.push_back([] {});
            //}
            //for (int i = 0; i < funcs.size(); i++) { if (funcs[i] == NULL) funcs[i] = [] {}; }
            for (int i = 0; i < tfuncs.size(); i++) {
                if (tfuncs[i] == NULL && funcs.find(i) == funcs.end())  funcs[i] = [] {};
                else                                                    funcs[i] = tfuncs[i];
            }

            setup(hInstance);
        }

        void setup(HINSTANCE hInstance, vec2 res) {
            cst::res(res);
            setup(hInstance);
        }

        void setup(HINSTANCE hInstance, vec2 res, std::vector<std::function<void()>> const& tfuncs) {
            cst::res(res);
            setup(hInstance, tfuncs);
        }

        void resize(vec2 size) { impl::context::resize(size); cst::res(size); }
        vec2 res() { return cst::res(); }
    }
}
