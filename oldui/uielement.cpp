#include "UIElement.h"

//namespace UIElement_UniqueTag{
//	unsigned long uiElementUniqueTag = 0;
//};

UIElement::UIElement(){
	//inputLayer = input::pollLayerId();
	inputTag = input::pollLayerId();
}

UIElement::UIElement(Vector2f pos, Vector2f size) : UIElement() {
	this->pos = pos;
	this->size = size;
}

UIElement::UIElement(Vector2f pos, Vector2f size, Vector4f color) : UIElement(pos, size) {
	this->color = color;
}

UIElement::~UIElement()
{
}

void UIElement::refresh() {

}

void UIElement::resize() {

}

void UIElement::setPos(Vector2f pos) {
	this->pos = pos;
}

Vector2f UIElement::getPos() {
	return pos;
}

void UIElement::setSize(Vector2f size) {
	this->size = size;
}

Vector2f UIElement::getSize() {
	return size;
}

void UIElement::setColor(Vector4f color) {
	this->color = color;
}

Vector4f UIElement::getColor() {
	return color;
}

void UIElement::update(){
	bool oldMouseInside = mouseInside;

	mouseInside = !disabled ? (checkOverlappingElems ? input::getMouseInQuad(pos, size, inputTag) : input::getMouseInQuadDefault(pos, size)) : false;

	if (oldMouseInside != mouseInside) for (auto& f1 : insideFuncs) f1(this, mouseInside);

	if (mouseInside) {
		if (input::getMouseDown(0, inputLayer)) {
			if(!isFocused) for (auto& f2 : focusFuncs) f2(this, true);
			isFocused = true;
		}
	}
	else {
		if (input::getMouseDown(0, inputLayer) || input::getMouseUp(0, inputLayer)) {
			if(isFocused) for (auto& f3 : focusFuncs) f3(this, false);
			isFocused = false;
		}
	}
}

void UIElement::altUpdate() {

}

void UIElement::draw() {

}

void UIElement::altDraw() {

}

void UIElement::dispose() {

}
