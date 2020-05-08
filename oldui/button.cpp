#include "Button.h"

#include "..\debug.h"

Button::Button(){
	color = Vector4f(0.8f, 0.8f, 0.8f, 1.0f);
}

Button::Button(TextRendererOld &txr) : Button() {
	
}

Button::Button(std::string text, TextRendererOld& txr, std::function<void()> action) : Button(text, txr, 0.f, 0.f, action) {}

Button::Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size) : Button(txr){
	this->pos = pos;
	this->size = size;
	label = Label(text, txr, pos, size);
	label.refreshTextBounds();
}

Button::Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size, std::function<void()> action) : Button(txr) {
	this->pos = pos;
	this->size = size;
	label = Label(text, txr, pos, size);
	this->action = action;
	label.refreshTextBounds();
}

Button::Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action) : Button(txr) {
	this->pos = pos;
	this->size = size;
	label = Label(text, textColor, txr, pos, size);
	this->color = backgroundColor;
	this->action = action;
	label.refreshTextBounds();
}

Button::Button(std::string text, float textScale , TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action) : Button(txr) {
	this->pos = pos;
	this->size = size;
	this->textScale = textScale;
	label = Label(text, textColor, txr, pos, size);
	this->color = backgroundColor;
	this->action = action;
	refresh();
}

Button::Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, Vector2f texCoord0, Vector2f texCoord1, std::function<void()> action) : Button(txr) {
	this->pos = pos;
	this->size = size;
	label = Label(text, textColor,1.0f, txr, pos, size);
	this->color = backgroundColor;
	this->texCoord0 = texCoord0;
	this->texCoord1 = texCoord1;
	this->action = action;
	label.refreshTextBounds();
}

Button::~Button()
{
}

void Button::refresh(){
	label.setTextScale(label.txr->getScreenPercScaleFactor(label.getSize().y*textScale));
	label.refresh();
}

void Button::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
		label.setPos(pos);
	}
}

void Button::setSize(Vector2f size) {
	this->size = size;
	label.setSize(size);
}

Vector2f Button::getSize() {
	return size;
}

void  Button::setTextScale(float textScale) {
	if (this->textScale != textScale) {
		this->textScale = textScale;
		refresh();
	}
}

float  Button::getTextScale() {
	return textScale;
}

void Button::update() {
	UIElement::update();

	if (input::getMouseDown(0, inputLayer)) {
		if (mouseInside) {
			locked = true;
		}
	}

	if (input::getMouseUp(0, inputLayer)) {
		if (locked) {
			if (mouseInside) {
				action();
			}

			locked = false;
		}
	}
}

void Button::draw() {
	drawBackground(this);

	label.draw();
}

void Button::draw(int &current, int &colCurrent, float *posBuf, float *colBuf, float *texBuf, TextRendererOld txr) {
	Vector4f tCol = Vector4f(color.x, color.y, color.z, color.w);
	if (locked) {
		tCol.x /= 4.0f;
		tCol.y /= 4.0f;
		tCol.z /= 4.0f;
	}
	else {
		if (mouseInside) {
			tCol.x /= 2.0f;
			tCol.y /= 2.0f;
			tCol.z /= 2.0f;
		}
	}

	posBuf[current + 0] = pos.x;			posBuf[current + 1] = pos.y;
	posBuf[current + 2] = pos.x + size.x;	posBuf[current + 3] = pos.y;
	posBuf[current + 4] = pos.x + size.x;	posBuf[current + 5] = pos.y + size.y;
	posBuf[current + 6] = pos.x;			posBuf[current + 7] = pos.y + size.y;

	colBuf[colCurrent + 0] = tCol.x;	colBuf[colCurrent + 1] = tCol.y;	colBuf[colCurrent + 2] = tCol.z;	colBuf[colCurrent + 3] = tCol.w;
	colBuf[colCurrent + 4] = tCol.x;	colBuf[colCurrent + 5] = tCol.y;	colBuf[colCurrent + 6] = tCol.z;	colBuf[colCurrent + 7] = tCol.w;
	colBuf[colCurrent + 8] = tCol.x;	colBuf[colCurrent + 9] = tCol.y;	colBuf[colCurrent + 10] = tCol.z;	colBuf[colCurrent + 11] = tCol.w;
	colBuf[colCurrent + 12] = tCol.x;	colBuf[colCurrent + 13] = tCol.y;	colBuf[colCurrent + 14] = tCol.z;	colBuf[colCurrent + 15] = tCol.w;

	texBuf[current + 0] = texCoord0.x;		texBuf[current + 1] = texCoord0.y;
	texBuf[current + 2] = texCoord1.x;		texBuf[current + 3] = texCoord0.y;
	texBuf[current + 4] = texCoord1.x;		texBuf[current + 5] = texCoord1.y;
	texBuf[current + 6] = texCoord0.x;		texBuf[current + 7] = texCoord1.y;
	
	current += 8;
	colCurrent += 15;

	//txr.accumulateString(displayText.c_str(), pos.x+textOffset.x, pos.y+textOffset.y, textScale);
}