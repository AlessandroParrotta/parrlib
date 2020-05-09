#include <parrlib/Input.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <parrlib/Context.h>
#include <parrlib/Vector2f.h>
#include <parrlib/constants.h>
#include <parrlib/debug.h>

#include <iostream>

#include <parrlib/physics2d/physutil2.h>

namespace input {
	void processInput(GLFWwindow *window);

	const int KEYS_LENGTH = 349;
	const int MOUSE_LENGTH = 12;

	bool oldKeys[KEYS_LENGTH];
	bool keys[KEYS_LENGTH];

	bool oldMouse[MOUSE_LENGTH];
	bool mouse[MOUSE_LENGTH];

	std::vector<unsigned int> textKeys;

	const int MOUSELEFT = 0, MOUSERIGHT = 1, MOUSEMIDDLE = 2;

	Vector2f rawMousePos;
	Vector2f mousePos;
	Vector2f disabledMousePos;
	Vector2f oldMousePos;
	Vector2f mouseDelta;

	int scrollWheel = 0;

	int cursorMode = GLFW_CURSOR_NORMAL;

	float aspectRatio = 0.0f;

	void init() {

		aspectRatio = cst::resaspect();
		for (int i = 0; i < KEYS_LENGTH; i++) {
			keys[i] = false;
			oldKeys[i] = keys[i];
		}

		for (int i = 0; i < MOUSE_LENGTH; i++) {
			mouse[i] = false;
			oldMouse[i] = keys[i];
		}
	}

	void processInput(GLFWwindow *window){
		/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);*/

		for (int i = 0; i < KEYS_LENGTH; i++) {
			if (glfwGetKey(window, i) == GLFW_PRESS) {
				keys[i] = true;
			}
			else {
				keys[i] = false;
			}
		}

		for (int i = 0; i < MOUSE_LENGTH; i++) {
			if (glfwGetMouseButton(window, i) == GLFW_PRESS) {
				mouse[i] = true;
			}
			else {
				mouse[i] = false;
			}
		}

		mouseDelta = mousePos - oldMousePos;
		oldMousePos = mousePos;
	}
	
	struct MouseLayer {
		Vector2f pos, size;
		int tag = 0;

		MouseLayer() {

		}

		MouseLayer(Vector2f pos, Vector2f size) {
			this->pos = pos;
			this->size = size;
		}

		MouseLayer(Vector2f pos, Vector2f size, int tag) {
			this->pos = pos;
			this->size = size;
			this->tag = tag;
		}
	};

	std::vector<MouseLayer> mouseLayers;
	std::vector<MouseLayer> mouseLayersCurFrame;
	int mLayerID = 0;

	/*std::vector<std::string> activeLayers(0);
	std::vector<std::string> mouseActiveLayers(0);

	std::string activeLayer = "default";
	std::string mouseActiveLayer = "default";*/

	std::vector<unsigned long> activeLayers;
	std::vector<unsigned long> mouseActiveLayers;

	void update() {
		for (int i = 0; i < KEYS_LENGTH; i++) {
			if (oldKeys[i] != keys[i]) {
				oldKeys[i] = keys[i];
			}
		}

		for (int i = 0; i < MOUSE_LENGTH; i++) {
			if (oldMouse[i] != mouse[i]) {
				oldMouse[i] = mouse[i];
			}
		}

		scrollWheel = 0;

		if (textKeys.size() > 0) {
			textKeys.clear();
		}

		if (mouseLayersCurFrame.size() > 0) {
			mouseLayers.clear();
			mouseLayers.insert(mouseLayers.begin() + 0, mouseLayersCurFrame.begin(), mouseLayersCurFrame.end());

			mouseLayersCurFrame.clear();
		}
		else {
			if (mouseLayers.size() > 0) {
				mouseLayers.clear();
			}
		}
		mLayerID = 0;

		/*activeLayers.clear();
		mouseActiveLayers.clear();*/

		//debug::rtPrintln((int)mouseLayers.size());
		//debug::rtPrintln((int)mouseLayersCurFrame.size());
	}
	
	unsigned long input_layers_ids = INPUT_LAYER_RESERVED;
	unsigned long pollLayerId() {
		input_layers_ids++;
		return input_layers_ids - 1;
	}

	/*void setActiveAlgorithm(std::string &activeLayer, std::string &rActiveLayer, std::vector<std::string> &vRef) {
		if (rActiveLayer.compare("default") == 0)  vRef.clear();
		if (rActiveLayer.compare(activeLayer) == 0) return;

		bool found = false;
		for (int i = vRef.size() - 1; i >= 0; i++) {
			if (activeLayer.compare(vRef[i]) == 0) {
				vRef.erase(vRef.begin() + i, vRef.end());
				found = true;
			}
		}

		if (!found) {
			vRef.push_back(rActiveLayer);
		}

		rActiveLayer = activeLayer;
	}*/

	//void setActiveLayer(std::string activeLayer) {
	//	//setActiveAlgorithm(activeLayer, input::activeLayer, activeLayers);
	//}

	//void setMouseActiveLayer(std::string mouseActiveLayer) {
	//	//setActiveAlgorithm(mouseActiveLayer, input::mouseActiveLayer, mouseActiveLayers);
	//}

	/*std::string getActiveLayer() {
		return activeLayer;
	}*/

	/*void resetActiveLayer() {
		if (activeLayers.size() > 0) {
			activeLayer = activeLayers.back();
			activeLayers.pop_back();
		}
		else {
			activeLayer = "default";
		}
	}*/

	/*void resetMouseActiveLayer() {
		if (mouseActiveLayers.size() > 0) {
			mouseActiveLayer = mouseActiveLayers.back();
			mouseActiveLayers.pop_back();
		}
		else {
			mouseActiveLayer = "default";
		}

	}*/

	int findActiveLayer(unsigned long layer) {
		int pos = -1;
		for (int i = 0; i < activeLayers.size() && pos == -1; i++) {
			if (layer == activeLayers[i])pos = i;
		}

		return pos;
	}

	void addActiveLayer(unsigned long layer) {
		if (layer == 0)return;

		int pos = findActiveLayer(layer);
		if (pos != -1)return;

		activeLayers.push_back(layer);
	}

	void removeActiveLayer(unsigned long layer) {
		if (layer == 0)return;

		int pos = findActiveLayer(layer);
		if (pos == -1)return;

		activeLayers.erase(activeLayers.begin() + pos);
	}

	bool isLayerActive(unsigned long layer) {
		if (layer == 0)return true;
		return findActiveLayer(layer) != -1;
	}

	bool getKey(int key, unsigned long layer) {
		if (layer == 0 || isLayerActive(layer)) {
			return keys[key];
		}
		else {
			return false;
		}
	}

	bool getKeyDown(int key, unsigned long layer) {
		if (layer==0 || isLayerActive(layer)) {
			return keys[key] && !oldKeys[key];
		}
		else {
			return false;
		}
	}

	bool getKeyUp(int key, unsigned long layer) {
		if (layer == 0 || isLayerActive(layer)) {
			return !keys[key] && oldKeys[key];
		}
		else {
			return false;
		}
	}

	bool getKey(int key) {
		return getKey(key, INPUT_LAYER_DEFAULT);
	}

	bool getKeyDown(int key) {
		return getKeyDown(key, INPUT_LAYER_DEFAULT);
	}

	bool getKeyUp(int key) {
		return getKeyUp(key, INPUT_LAYER_DEFAULT);
	}

	/*bool callOrFor(int key, std::vector<unsigned long> layers, std::function<bool(int key, unsigned long layer)> func) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = func(key, layers[i]);
		}
		return result;
	}*/

	bool getKey(int key, std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getKey(key, layers[i]);
		}
		return result;
	}

	bool getKeyDown(int key, std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getKeyDown(key, layers[i]);
		}
		return result;
	}

	bool getKeyUp(int key, std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getKeyUp(key, layers[i]);
		}
		return result;
	}

	int getAnyKeyDown(unsigned long layer) {
		if (layer == 0 || isLayerActive(layer)) {
			for (int i = 0; i < KEYS_LENGTH; i++) {
				if (getKeyDown(i, layer)) {
					return i;
				}
			}
		}

		return -1;
	}

	int getAnyKey(unsigned long layer) {
		if (layer == 0 || isLayerActive(layer)) {
			for (int i = 0; i < KEYS_LENGTH; i++) {
				if (getKey(i, layer)) {
					return i;
				}
			}
		}

		return -1;
	}

	int getAnyKeyUp(unsigned long layer) {
		if (layer == 0 || isLayerActive(layer)) {
			for (int i = 0; i < KEYS_LENGTH; i++) {
				if (getKeyUp(i, layer)) {
					return i;
				}
			}
		}

		return -1;
	}

	int getAnyKeyDown(std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getAnyKeyDown(layers[i]);
		}
		return result;
	}

	int getAnyKey(std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getAnyKey(layers[i]);
		}
		return result;
	}

	int getAnyKeyUp(std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getAnyKeyUp(layers[i]);
		}
		return result;
	}

	std::vector<int> getPressedKeys() {
		std::vector<int> keys;
		for (int i = 0; i < KEYS_LENGTH; i++) {
			if (getKey(i)) keys.push_back(i);
			
		}
		return keys;
	}
	
	std::vector<int> getPressedKeysDown() {
		std::vector<int> keys;
		for (int i = 0; i < KEYS_LENGTH; i++) {
			if (getKeyDown(i)) keys.push_back(i);
			
		}
		return keys;
	}
	
	std::vector<int> getPressedKeysUp() {
		std::vector<int> keys;
		for (int i = 0; i < KEYS_LENGTH; i++) {
			if (getKeyUp(i)) keys.push_back(i);
			
		}
		return keys;
	}

	unsigned int pollTextKey() {
		if (textKeys.size() > 0) {
			unsigned int val = textKeys[textKeys.size() - 1];
			//textKeys.erase(textKeys.begin() + (textKeys.size()-1));
			textKeys.pop_back();
			return val;
		}
		
		return 0;
	}

	int getAnyKeyDown() {
		return getAnyKeyDown(INPUT_LAYER_DEFAULT);
	}

	int getAnyKey() {
		return getAnyKey(INPUT_LAYER_DEFAULT);
	}

	int getAnyKeyUp() {
		return getAnyKeyUp(INPUT_LAYER_DEFAULT);
	}

	bool getMouse(int button, unsigned long layer) {
		return (layer == 0 || isLayerActive(layer)) && mouse[button];
	}

	bool getMouseDown(int button, unsigned long layer) {
		return (layer == 0 || isLayerActive(layer)) && mouse[button] && !oldMouse[button];
	}

	bool getMouseUp(int button, unsigned long layer) {
		return (layer == 0 || isLayerActive(layer)) && !mouse[button] && oldMouse[button];
	}

	int getMouseScrollWheel(unsigned long layer) {
		return (layer == 0 || isLayerActive(layer)) ? scrollWheel : 0;
	}

	bool getMouse(int button) {
		return getMouse(button, INPUT_LAYER_DEFAULT);
	}

	bool getMouseDown(int button) {
		return getMouseDown(button, INPUT_LAYER_DEFAULT);
	}

	bool getMouseUp(int button) {
		return getMouseUp(button, INPUT_LAYER_DEFAULT);
	}

	int getMouseScrollWheel() {
		return getMouseScrollWheel(INPUT_LAYER_DEFAULT);
	}

	bool getMouse(int button, std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getMouse(button, layers[i]);
		}
		return result;
	}

	bool getMouseDown(int button, std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getMouseDown(button, layers[i]);
		}
		return result;
	}

	bool getMouseUp(int button, std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getMouseUp(button, layers[i]);
		}
		return result;
	}

	bool leftClick()		{ return getMouseUp(0); }
	bool rightClick()		{ return getMouseUp(1); }
	bool click()			{ return leftClick(); }

	bool leftClickDown()	{ return getMouseDown(0); }
	bool rightClickDown()	{ return getMouseDown(1); }
	bool clickDown()		{ return getMouseDown(0); }

	int getMouseScrollWheel(std::vector<unsigned long> layers) {
		bool result = false;
		for (int i = 0; i < layers.size() && !result; i++) {
			result = getMouseScrollWheel(layers[i]);
		}
		return result;
	}

	Vector2f getMousePosNdc() {
		if (getCursorInputMode() == GLFW_CURSOR_DISABLED) return disabledMousePos;
		else return (mousePos/prc::wres()*2.f-1.f).ny();
	}

	vec2 getMousePos() { return util::getAspectOrtho().inverted() * (!prc::inApp ? 1.f : pmat3::getNdc(prc::sbb)) * getMousePosNdc(); }

	vec2 getIAxis2() {
		return vec2(
			input::getKey(GLFW_KEY_D) - input::getKey(GLFW_KEY_A),
			input::getKey(GLFW_KEY_W) - input::getKey(GLFW_KEY_S)
		);
	}
	
	vec3 getIAxis3() {
		return vec3(
			input::getKey(GLFW_KEY_D) - input::getKey(GLFW_KEY_A),
			input::getKey(GLFW_KEY_SPACE) - input::getKey(GLFW_KEY_C),
			input::getKey(GLFW_KEY_W) - input::getKey(GLFW_KEY_S)
		);
	}

	struct inputcombo {
		std::vector<unsigned int> keys;
	};
	std::unordered_map<std::string, std::vector<inputcombo>> actions;

	bool runActionFunc(std::string const& ac, bool (*f)(int)) {
		bool res = false;

		for (auto& ic : actions[ac]) {
			for (auto& k : ic.keys) {
				res &= f(k);
				if (!res) break;
			}

			if (res) return true;
		}

		return res;
	}

	bool getAction(std::string const& ac) {
		return runActionFunc(ac, &getKey);
	}

	bool getActionDown(std::string const& ac) {
		return runActionFunc(ac, &getKeyDown);
	}

	bool getActionUp(std::string const& ac) {
		return runActionFunc(ac, &getKeyUp);
	}

	float getMouseX() {
		return mousePos.x;
	}

	float getMouseY() {
		return mousePos.y;
	}

	/*float getMouseYInv() {
		return constants::HEIGHT - mousePos.y;
	}*/

	/*Vector2f getMousePosInv() {
		return Vector2f(getMousePos().x, constants::HEIGHT - getMousePos().y);
	}*/

	Vector2f getRawMousePos() {
		return rawMousePos;
	}

	Vector2f getRawMousePosInv() {
		return Vector2f(getRawMousePos().x, prc::wsize.y - getRawMousePos().y);
	}

	/*Vector2f getMousePerc() {
		return Vector2f((getMousePos().x - constants::WIDTH / 2.0f) / (constants::WIDTH / 2.0f), 0.0f - (getMousePos().y - (constants::HEIGHT / 2.0f)) / ((constants::HEIGHT / 2.0f)));
	}

	Vector2f getMouseOrtho() {
		Vector2f m = getMousePos();
		return Vector2f(prc::orthoLeft + (prc::orthoRight - prc::orthoLeft)*((m.x)/(constants::WIDTH)), 
						prc::orthoBottom + (prc::orthoTop - prc::orthoBottom)*(1.0f - (m.y) / (constants::HEIGHT)));
	}*/

	/*Vector2f getMouseCoordsNormalized() {
		return Vector2f((getMousePos().x - constants::WIDTH / 2.0f) / (constants::WIDTH / 2.0f), 0.0f - (getMousePos().y - (constants::HEIGHT / 2.0f)) / ((constants::HEIGHT / 2.0f))*aspectRatio);
	}

	Vector2f getMouseCoords() {
		return getMouseCoordsNormalized() - util::camPos;
	}*/

	Vector2f getMouseDeltaConst() {
		return mouseDelta / vec2(1920.f, 1080.f)*60.f; //180.f/3.f, about 180deg in 20cm in a 1920x1080 screen with default windows mouse settings, feels right to me
	}

	Vector2f getMouseDelta() {
		return (mouseDelta/cst::res()*2.f*cst::res().aspectmaxv()).ny();
	}

	Vector2f getRawMouseDelta() {
		return mouseDelta;
	}

	/*Vector2f getMouseDeltaOrtho() {
		return Vector2f((mouseDelta.x / (float)constants::WIDTH) * (prc::orthoRight - prc::orthoLeft), 
						(mouseDelta.y / (float)constants::HEIGHT) * (prc::orthoTop - prc::orthoBottom));
	}

	Vector2f getMouseDeltaCoords() {
		return Vector2f(mouseDelta.x/(float)constants::WIDTH, mouseDelta.y/(float)constants::HEIGHT);
	}*/

	//values can be GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN
	void setCursorInputMode(int inputMode) {
		Vector2f prevMousePos = getMousePos();
		cursorMode = inputMode;
		glfwSetInputMode(prc::glfwwindow, GLFW_CURSOR, inputMode);

		if (inputMode == GLFW_CURSOR_DISABLED) {
			disabledMousePos = prevMousePos;
		}
	}

	int getCursorInputMode() {
		return cursorMode;
	}

	void toggleCursorInputMode() {
		if (getCursorInputMode() == GLFW_CURSOR_DISABLED) setCursorInputMode(GLFW_CURSOR_NORMAL);
		else if (getCursorInputMode() == GLFW_CURSOR_NORMAL) setCursorInputMode(GLFW_CURSOR_DISABLED);
	}

	bool getMouseInQuadDefault(Vector2f quadPos, Vector2f quadSize) {
		//Vector2f mouse = getMouseCoords();
		//Vector2f mouse = getMouseOrtho();
		Vector2f mouse = getMousePos();

		return mouse.x >= quadPos.x && mouse.x <= quadPos.x + quadSize.x &&
				mouse.y >= quadPos.y && mouse.y <= quadPos.y + quadSize.y;
	}

	bool getMouseInQuad(Vector2f quadPos, Vector2f quadSize, int tag) {
		bool inQuad = false;
		if (mouseLayers.size() == 0) {
			inQuad = getMouseInQuadDefault(quadPos, quadSize);
		}
		else {
			if (getMouseInQuadDefault(quadPos, quadSize)) {
				
				inQuad = true;
				for (int i = mLayerID+1; i < mouseLayers.size() && inQuad; i++) {
					if (tag >= 0) {
						inQuad = !(getMouseInQuadDefault(mouseLayers[i].pos, mouseLayers[i].size) && mouseLayers[i].tag != tag);
					}
					else {
						inQuad = !getMouseInQuadDefault(mouseLayers[i].pos, mouseLayers[i].size);
					}
				}
			}
		}

		mouseLayersCurFrame.push_back(MouseLayer(quadPos, quadSize, tag));
		mLayerID++;

		return inQuad;
	}

	bool getMouseInQuad(Vector2f quadPos, Vector2f quadSize) {
		return getMouseInQuad(quadPos, quadSize, -1);
	}

	bool mouseInQuad(aabb2 const& bb) { return psu2::checkCollisionb(input::getMousePos(), bb); }
	bool mouseInQuad(vec2 const& pos, vec2 const& size) { return mouseInQuad({pos, pos+size}); }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>

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
		wchar_t *cText = static_cast<wchar_t*>(GlobalLock(hData));

		std::wstring clipboard = std::wstring(cText);

		GlobalUnlock(hData);

		CloseClipboard();

		return clipboard;
	}

#else

	bool getCapsLockStatus() {
		return false;
	}

	void copyToClipboard(std::wstring text) {
		return;
	}

	std::wstring getFromClipboardw() {
		return L"";
	}

#endif

	void copyToClipboard(std::string text)	{ copyToClipboard(stru::towstr(text)); }
	std::string getFromClipboard()			{ return stru::tostr(getFromClipboardw()); }
}