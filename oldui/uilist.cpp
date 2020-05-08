#include "UIList.h"

UIList::UIList() {}
UIList::UIList(vec2 pos, vec2 size) : UIElement(pos, size, 0.f) {}

//elemSize is percentage of size
UIList::UIList(vec2 pos, vec2 size, vec2 elemSize) : UIElement(pos, size, 0.f), elemWidth(elemSize.x), elemHeight(elemSize.y){}

void UIList::set(std::string id, std::shared_ptr<UIElement> elem) {
	elems[id] = elem;
	setPos(pos);
	setSize(size);
}

void UIList::add(std::string id, std::shared_ptr<UIElement> elem) { set(id, elem); }

void UIList::setPos(vec2 pos) {
	UIElement::setPos(pos);
	int i = 0;
	for (auto& e : elems) {
		e.second->setPos(vec2(pos.x + size.x/2.f - (elemWidth/2.f) + padding, pos.y + size.y - (elemHeight - spacing) * (i+1) + padding));
		//e.second->setPos(vec2(pos.x + size.x/2.f - (size.x*elemWidth/2.f) + padding, pos.y + size.y - ((size.y + spacing) * elemHeight) * (i+1) + padding));
		i++;
	}
}

void UIList::setSize(vec2 size) {
	setPos(pos);
	UIElement::setSize(size);
	for (auto& e : elems) {
		e.second->setSize(vec2(elemWidth-padding*2.f, elemHeight-padding*2.f));
		//e.second->setSize(vec2(size.x*elemWidth-padding*2.f, size.y*elemHeight-padding*2.f));
	}
}

void UIList::resize() {
	setPos(pos);
	setSize(size);
}

void UIList::setElemHeight(float elemHeight) {
	this->elemHeight = elemHeight;
	resize();
}

float UIList::getElemHeight() {
	return elemHeight;
}

void UIList::setElemWidth(float elemWidth) {
	this->elemWidth = elemWidth;
	resize();
}

float UIList::getElemWidth() {
	return elemWidth;
}

void UIList::setSpacing(float spacing) {
	this->spacing = spacing;
	resize();
}

float UIList::getSpacing() {
	return spacing;
}

void UIList::setPadding(float padding) {
	this->padding = padding;
	resize();
}

float UIList::getPadding() {
	return padding;
}

void UIList::refresh() {
	for (auto& e : elems) {
		e.second->refresh();
	}
}

void UIList::update() {
	for (auto& e : elems) {
		e.second->update();
	}
}

void UIList::draw() {
	util::pushGLScissor(pos, size);

		drawBackground(this);
		//int i = 0;
		for (auto& e : elems) {
			e.second->draw();

			//util::setColor4f(vColor4f::black);
			//util::drawQuad(vec2(pos.x + size.x / 2.f - (size.x*elemWidth) / 2.f, pos.y + size.y - ((size.y + spacing) * elemHeight) * (i + 1)), vec2(size.x*elemWidth, size.y*elemHeight), GL_LINE_LOOP);
			//i++;
		}

	util::popGLScissor();
}

void UIList::altDraw() {
	util::pushGLScissor(pos, size);

		for (auto& e : elems) {
			e.second->altDraw();
		}

	util::popGLScissor();
}

std::shared_ptr<UIElement> UIList::operator[](std::string id) {
	if (elems.find(id) == elems.end()) return nullptr;
	return elems[id];
}
