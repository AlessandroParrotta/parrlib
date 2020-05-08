#include "DropMenu.h"

#include "..\debug.h"


DropMenu::DropMenu()
{
}

DropMenu::DropMenu(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size) : Button(text, txr, pos, size) {
	refreshDropMenu();
}

DropMenu::DropMenu(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor) : Button(text, textScale, txr, pos, size, backgroundColor, textColor, [] {}) {
	refreshDropMenu();
}

DropMenu::DropMenu(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::vector<Element> elements) : Button(text, textScale, txr, pos, size, backgroundColor, textColor, [] {}) {
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i].isDropMenu) {
			elements[i].dm->label.setTextScale(textScale);
			elements[i].dm->color = backgroundColor;
			elements[i].dm->label.setColor(textColor);
			addElem(elements[i].dm);
			dynamic_cast<DropMenu*>(this->elements[this->elements.size()-1])->refreshDropMenu();
		}
		else{
			addElem(elements[i].str, elements[i].action);
		}
	}

	Vector2f opos = pos, osize = size;

	setPos(pos + 1.0f);
	setSize(pos + 1.0f);

	setPos(opos);
	setSize(osize);

	refreshDropMenu();
}

DropMenu::DropMenu(std::string text, TextRendererOld &txr, std::vector<Element> elements) : DropMenu(text, 1.0f, txr, Vector2f(0.0f), Vector2f(0.0f), Vector4f(0.0f), Vector4f(0.0f), elements) {

}

DropMenu::DropMenu(std::string text, float textScale, TextRendererOld &txr, std::vector<Element> elements) : DropMenu(text, textScale, txr, Vector2f(0.0f), Vector2f(0.0f), Vector4f(0.0f), Vector4f(0.0f), elements) {

}

void DropMenu::refreshDropMenu() {
	label.setTextScale(label.txr->getScreenPercScaleFactor(label.getSize().y *textScale));
	label.refreshTextBounds();

	float unitY = getSize().y;

	if (openSide == OpenSide::RIGHT) {
		menuSize.x = 0.2f;
		menuSize.y = unitY * elements.size();
	}
	else if (openSide == OpenSide::DOWN) {
		menuSize.x = size.x;
		menuSize.y = unitY * elements.size();
	}

	for (int i = 0; i < elements.size(); i++) {
		//menuSize.x = std::fmax(menuSize.x, label.txr->getStringBounds(elements[i]->label.displayText.c_str(), label.textScale).size.x);
		if (openSide == OpenSide::RIGHT) {
			elements[i]->setPos(Vector2f(pos.x + getSize().x, pos.y - unitY * i));
		}
		else if (openSide == OpenSide::DOWN) {
			elements[i]->setPos(Vector2f(pos.x, pos.y - unitY - unitY * i));
		}
		elements[i]->setSize(Vector2f(menuSize.x, unitY));
		elements[i]->setTextScale(0.6f);
		elements[i]->label.setTextScale(label.getTextScale());
		elements[i]->refresh();
		elements[i]->setColor(color);
		elements[i]->label.setColor(label.getColor());
		elements[i]->label.setTextScale(label.getTextScale());
		elements[i]->label.refreshTextBounds();
	}

	Vector2f openOffset;
	if (openSide == OpenSide::RIGHT) {
		openOffset = Vector2f(-size.x*openSideButtonSizeOff, 0.0f);

		label.setSize(size + openOffset);
		label.setPos(pos);

		openSideButtonPos = label.getPos() + Vector2f(label.getSize().x, 0.0f);
		openSideButtonSize = Vector2f(-openOffset.x, size.y);

		openSideButtonText = ">";
	}
	else if (openSide == OpenSide::DOWN) {
		openOffset = Vector2f(0.0f, -size.y*openSideButtonSizeOff);

		label.setSize(size + openOffset);
		label.setPos(pos - openOffset);

		openSideButtonPos = pos;
		openSideButtonSize = Vector2f(size.x, -openOffset.y);

		openSideButtonText = "v";
	}

	openSideButtonScale = std::fmin(label.getTextScale(), label.txr->getScaleToFit(openSideButtonText, openSideButtonSize));
	openSideButtonBound = label.txr->getStringBounds(openSideButtonText, openSideButtonScale);

	/*for (int i = 0; i < elements.size(); i++) {
		elements[i]->setSize(Vector2f(menuSize.x, unitY));
	}*/
}

void DropMenu::refresh() {
	refreshDropMenu();
}

void DropMenu::addElem(std::string text, std::function<void()> action) {
	elements.push_back(new Button(text, label.getTextScale(), *label.txr, Vector2f(0.0f), Vector2f(0.0f), color, label.getColor(), action));
	refreshDropMenu();
}

void DropMenu::addElem(Button *element) {
	if (dynamic_cast<DropMenu*>(element) != NULL) {
		((DropMenu*)element)->setOpenSide(DropMenu::OpenSide::RIGHT);
	}
	elements.push_back(element);
	refreshDropMenu();
}

void DropMenu::removeElem(int index) {
	delete elements[index];
	elements.erase(elements.begin() + index);
	refreshDropMenu();
}

void DropMenu::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
		label.setPos(pos);
		refreshDropMenu();
	}
}

void DropMenu::setSize(Vector2f size) {
	this->size = size;
	refreshDropMenu();
}


void DropMenu::setOpen(bool open) {
	this->open = open;

	if (!open) {
		for (int i = 0; i < elements.size(); i++) {
			if (dynamic_cast<DropMenu*>(elements[i]) != NULL) {
				DropMenu *d = dynamic_cast<DropMenu*>(elements[i]);
				d->open = false;
				debug::println("closed!");
			}
		}
	}
}

void DropMenu::setOpenSide(OpenSide openSide) {
	if (this->openSide != openSide) {
		this->openSide = openSide;
		refreshDropMenu();
	}
}

DropMenu::OpenSide DropMenu::getOpenSide() {
	return openSide;
}

bool DropMenu::isMouseInChildren() {
	for (int i = 0; i < elements.size(); i++) {
		if (dynamic_cast<DropMenu*>(elements[i]) != NULL) {
			DropMenu *dm = dynamic_cast<DropMenu*>(elements[i]);

			if (input::getMouseInQuad(elements[i]->getPos(), elements[i]->getSize()) || dm->isMouseInChildren()) {
				return true;
			}
		}
		else {
			if (input::getMouseInQuad(elements[i]->getPos(), elements[i]->getSize())) {
				return true;
			}
		}
	}

	return false;
}

void DropMenu::update() {
	UIElement::update();

	if (input::getMouseDown(0, inputLayer)) {
		if (mouseInside) {
			setOpen(!open);

			locked = true;
		}
		else {
			if (open) {
				if (!isMouseInChildren()) {
					setOpen(false);
				}
			}
		}
	}

	if (input::getMouseUp(0, inputLayer)) {
		if (locked) {
			locked = false;
		}
	}

	if (open) {
		for (int i = 0; i < elements.size(); i++) {
			elements[i]->update();
		}
	}
}

void DropMenu::draw() {
	drawBackground(this);

	Button::draw();

	Vector4f col = label.getColor();
	glColor4f(col.x, col.y, col.z, col.w);
	label.txr->drawString(openSideButtonText, openSideButtonPos + openSideButtonSize/2.0f - openSideButtonBound.size/2.0f + openSideButtonBound.offset, openSideButtonScale);
}

void DropMenu::altDraw() {
	if (open) {
		//glColor4f(0.5f,0.5f,0.5f,1.0f);
		//util::drawQuad(pos + Vector2f(0.0f, getSize().y) + Vector2f(getSize().x, -menuSize.y), menuSize);

		for (int i = 0; i < elements.size(); i++) {
			elements[i]->draw();
		}
	}
}


DropMenu::~DropMenu()
{
}
