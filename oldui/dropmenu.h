#pragma once

#include <vector>

#include "Button.h"

class DropMenu : public Button {
protected:
	Vector2f menuSize;

public:

	std::vector<Button*> elements;
	bool open = false;

	enum OpenSide {
		RIGHT,
		DOWN
	};
protected:
	OpenSide openSide = OpenSide::RIGHT;
	float openSideButtonSizeOff = 0.2f;

	Vector2f openSideButtonPos, openSideButtonSize;

	std::string openSideButtonText = ">";
	float openSideButtonScale = 1.0f;
	TextRendererOld::StringBound openSideButtonBound;

public:

	struct Element {
		std::string str;
		std::function<void()> action;
		DropMenu *dm;
		bool isDropMenu = false;

		Element() {

		}

		Element(std::string str, std::function<void()> action) {
			this->str = str;
			this->action = action;
		}

		Element(DropMenu *dm) {
			this->dm = dm;
			isDropMenu = true;
		}
	};

	DropMenu();
	DropMenu(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size);
	DropMenu(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor);
	DropMenu(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::vector<Element> elements);
	
	DropMenu(std::string text, TextRendererOld &txr, std::vector<Element> elements);
	DropMenu(std::string text, float textScale, TextRendererOld &txr, std::vector<Element> elements);

	void refreshDropMenu();
	void refresh();

	void addElem(Button *element);
	void addElem(std::string text, std::function<void()> action);

	void removeElem(int index);

	virtual void setPos(Vector2f pos);
	virtual void setSize(Vector2f size);

	void setOpen(bool open);

	void setOpenSide(OpenSide openSide);
	OpenSide getOpenSide();

	bool isMouseInChildren();

	virtual void update();
	virtual void draw();
	virtual void altDraw();

	~DropMenu();
};

