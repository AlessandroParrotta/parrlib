#pragma once
#include <vector>

#include "UIElement.h"

class UIManager {
protected:
	std::vector<UIElement*> uiElems;

public:
	UIManager();
	UIManager(std::vector<UIElement*> &uiElems);

	~UIManager();

	void addElem(int at, UIElement* uiElem);
	void addElem(UIElement* uiElem);

	void removeElem(int i);
	void removeElems(int from, int to);

	UIElement* at(int i);

	int getElemCount();

	void update();
	void draw();

	void dispose();
};

