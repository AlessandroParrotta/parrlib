#pragma once

#define INPUT_LAYER_UNFILTERED	0
#define INPUT_LAYER_DEBUG		INPUT_LAYER_UNFILTERED+1
#define INPUT_LAYER_DEBUGMENU	INPUT_LAYER_DEBUG+1
#define INPUT_LAYER_DEFAULT		INPUT_LAYER_DEBUGMENU+1
#define INPUT_LAYER_RESERVED	INPUT_LAYER_DEFAULT+1

#include <vector>
#include <functional>

#include "vector2f.h"
#include "vector4f.h"
#include "orthocoord.h"
#include "util.h"

namespace input {

	extern const int MOUSELEFT, MOUSERIGHT, MOUSEMIDDLE;

	extern Vector2f rawMousePos;
	extern Vector2f mousePos;
	extern int scrollWheel;

	extern Vector2f disabledMousePos;

	extern std::vector<unsigned int> textKeys;

	void init();

	void processInput(GLFWwindow *window);

	void update();
	
	unsigned long pollLayerId();

	/*void setActiveLayer(std::string activeLayer);
	void setMouseActiveLayer(std::string mouseActiveLayer);
	void resetActiveLayer();
	void resetMouseActiveLayer();
	std::string getActiveLayer();*/

	void addActiveLayer(unsigned long layer);
	void removeActiveLayer(unsigned long layer);
	bool isLayerActive(unsigned long layer);

	bool getKey(int key, unsigned long layer);
	bool getKeyDown(int key, unsigned long layer);
	bool getKeyUp(int key, unsigned long layer);
	bool getKey(int key, std::vector<unsigned long> layers);
	bool getKeyDown(int key, std::vector<unsigned long> layers);
	bool getKeyUp(int key, std::vector<unsigned long> layers);
	bool getKey(int key);
	bool getKeyDown(int key);
	bool getKeyUp(int key);

	int getAnyKeyDown(unsigned long layer);
	int getAnyKey(unsigned long layer);
	int getAnyKeyUp(unsigned long layer);
	int getAnyKeyDown(std::vector<unsigned long> layers);
	int getAnyKey(std::vector<unsigned long> layers);
	int getAnyKeyUp(std::vector<unsigned long> layers);
	int getAnyKeyDown();
	int getAnyKey();
	int getAnyKeyUp();

	std::vector<int> getPressedKeys();
	std::vector<int> getPressedKeysDown();
	std::vector<int> getPressedKeyssUp();

	unsigned int pollTextKey();

	bool getMouse(int button, unsigned long layer);
	bool getMouseDown(int button, unsigned long layer);
	bool getMouseUp(int button, unsigned long layer);
	int getMouseScrollWheel(unsigned long layer);

	bool getMouse(int button);
	bool getMouseDown(int button);
	bool getMouseUp(int button);
	int getMouseScrollWheel();

	bool getMouse(int button, std::vector<unsigned long> layers);
	bool getMouseDown(int button, std::vector<unsigned long> layers);
	bool getMouseUp(int button, std::vector<unsigned long> layers);
	int getMouseScrollWheel(std::vector<unsigned long> layers);

	bool leftClick();	//alias for input::getMouseUp(0)
	bool rightClick();	//alias for input::getMouseUp(1)
	bool click();		//alias for input::getMouseUp(0)

	bool leftClickDown();	//alias for input::getMouseDown(0)
	bool rightClickDown();	//alias for input::getMouseDown(1)
	bool clickDown();		//alias for input::getMouseDown(0)

	Vector2f getMousePosNdc();
	vec2 getMousePos();

	bool getAction(std::string const& ac);
	bool getActionDown(std::string const& ac);
	bool getActionUp(std::string const& ac);

	vec2 getIAxis2();
	vec3 getIAxis3();

	//Vector2f getMousePosInv();

	Vector2f getRawMousePos();
	Vector2f getRawMousePosInv();

	float getMouseX();
	float getMouseY();

	//float getMouseYInv();

	//Vector2f getMouseCoordsNormalized();

	//Vector2f getMousePerc();

	//Vector2f getMouseOrtho();

	//Vector2f getMouseCoords();

	Vector2f getMouseDeltaConst();
	Vector2f getMouseDelta();
	Vector2f getRawMouseDelta();
	//Vector2f getMouseDeltaOrtho();
	//Vector2f getMouseDeltaCoords();


	void setCursorInputMode(int inputMode);

	int getCursorInputMode();

	void toggleCursorInputMode();

	bool getMouseInQuadDefault(Vector2f quadPos, Vector2f quadSize);
	bool getMouseInQuad(Vector2f quadPos, Vector2f quadSize);
	bool getMouseInQuad(Vector2f quadPos, Vector2f quadSize, int tag);

	bool mouseInQuad(aabb2 const& bb);
	bool mouseInQuad(vec2 const& pos, vec2 const& size);

	//bool getMouseInQuadLayered(Vector2f quadPos, Vector2f quadSize);

	bool getCapsLockStatus();

	void copyToClipboard(std::wstring text);
	std::wstring getFromClipboardw();
	
	void copyToClipboard(std::wstring text);
	std::string getFromClipboard();
}