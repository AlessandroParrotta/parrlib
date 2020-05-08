#pragma once

#include <functional>

#include "..\Vector2f.h"
#include "..\Vector4f.h"
#include "..\Input.h"
#include "..\Time.h"

#include "..\debug.h"

//namespace UIElement_UniqueTag {
//	extern unsigned long uiElementUniqueTag;
//};

class UIElement {
protected:
	Vector2f pos, size;
	Vector4f color, backgroundColor=vColor4f::gray.colMul(1.1f);

public:
	unsigned long inputLayer = INPUT_LAYER_DEFAULT;
	unsigned long inputTag = INPUT_LAYER_DEFAULT;
	bool mouseInside = false;
	bool isFocused = false;
	bool checkOverlappingElems = true;
	bool disabled = false;

	std::vector<std::function<void(UIElement*, bool)>> focusFuncs;
	std::vector<std::function<void(UIElement*, bool)>> insideFuncs;

	UIElement();
	UIElement(Vector2f pos, Vector2f size);
	UIElement(Vector2f pos, Vector2f size, Vector4f color);

	~UIElement();

	virtual void refresh();

	virtual void resize();

	virtual void setPos(Vector2f pos);
	virtual Vector2f getPos();

	virtual void setSize(Vector2f size);
	virtual Vector2f getSize();

	virtual void setColor(Vector4f color);
	virtual Vector4f getColor();

	virtual void update();
	virtual void altUpdate();

	std::function<void(UIElement *caller)> drawBackground = [](UIElement *caller) {
		if (caller->color.w > 0.f) {
			util::blendEnable();
			util::setColor4f(caller->backgroundColor.colMul(caller->mouseInside ? (input::getMouse(0) ? (1.2f) : 1.1f) : 1.f));
			util::drawQuad(caller->pos, caller->size);
			util::blendDisable();
		}
	};

	virtual void draw();
	virtual void altDraw();

	virtual void dispose();
};

