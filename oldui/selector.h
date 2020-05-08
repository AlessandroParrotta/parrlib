#pragma once

#include <functional>

#include "..\Vector2f.h"
#include "..\TextRendererOld.h"
#include "..\debug.h"

#include "UIElement.h"
#include "Button.h"
#include "TextField\TextField.h"

class Selector : public UIElement{
protected:
	float symbolOff = 0.1f;
	Vector2f symbolPos, symbolSize;

	float symbolScale;
	TextRendererOld::StringBound symbolBound;

	Vector4f labelCol;

public:
	struct Element {
		std::string text;
		std::function<void()> action;

		Element() {

		}

		Element(std::string text, std::function<void()> action) {
			this->text = text;
			this->action = action;
		}
	};

protected:
	std::vector<Element> elements;
	int selectedElement = -1;

public:
	TextField txf;
	Label description;

	std::function<void(std::string)> action;
	bool open = false;
	
	std::vector<Button*> buttons;
	std::vector<Button*> filteredButtons;

	Selector();
	Selector(std::string text, std::vector<Element> elements, TextRendererOld& txr, std::function<void(std::string)> action);
	Selector(std::string text, std::vector<Element> elements, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void(std::string)> action);

	~Selector();

	virtual void refresh();

	virtual void setPos(Vector2f pos);
	virtual void setSize(Vector2f size);

	virtual void setSelectedElement(int selectedElement, bool invokeAction);
	virtual void setSelectedElement(int selectedElement);

	virtual int getSelectedElementIdx();
	virtual Element &getSelectedElement();
	virtual Element &getElement(int elem);

	virtual void update();
	virtual void draw();
	virtual void altDraw();

	virtual void dispose();
};

