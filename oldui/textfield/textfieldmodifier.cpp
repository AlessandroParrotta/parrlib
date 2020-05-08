//#include "stdafx.h"
#include "TextFieldModifier.h"


TextFieldModifier::TextFieldModifier()
{
}

TextFieldModifier::TextFieldModifier(TextRendererOld &txr) : TextField(txr){
	//label.setAlignment(Label::Alignment::RIGHT);
}

TextFieldModifier::TextFieldModifier(float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, TextField::Constraint constraint, std::function<void()> action) : TextField("", textScale, txr, pos, size, backgroundColor, textColor, constraint, action) {
	//label.setAlignment(Label::Alignment::RIGHT);

	Vector2f oldPos = pos, oldSize = size;
	pos += 0.01f;
	size += 0.01f;

	setPos(pos);
	setSize(size);

	setPos(oldPos);
	setSize(oldSize);
}

TextFieldModifier::TextFieldModifier(float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, TextField::Constraint constraint) : TextFieldModifier(textScale, txr, pos, size, backgroundColor, textColor, constraint, [] {}) {
	
}

TextFieldModifier::~TextFieldModifier()
{
}

void TextFieldModifier::refreshTextField() {
	TextField::refreshTextField();

	if (modifierEnabled) {
		modifierPos = label.getPos() + Vector2f((size.x - labelSizeOffset.x*2.0f)*modifierOffset + modifierSpacing, 0.0f);
		modifierSize = Vector2f((size.x - labelSizeOffset.x*2.0f)*(1.0f - modifierOffset) - modifierSpacing, size.y - labelSizeOffset.y*2.0f);
	}
	else {
		modifierPos = label.getPos() + Vector2f(size.x - labelSizeOffset.x*2.0f, 0.0f);
		modifierSize = Vector2f(0.0f);
	}

	label.setSize(size - labelSizeOffset * 2.0f - Vector2f(modifierSize.x, 0.0f) - Vector2f((modifierEnabled ? size.x*modifierSpacing : 0.0f), 0.0f));

	modifierScale = txr->getScaleToFit(std::string("<|>"), modifierSize);
	modifierBound = txr->getStringBounds(std::string("<|>"), modifierScale);
}

void TextFieldModifier::refresh() {
	refreshTextField();
}

void TextFieldModifier::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
		inputLayer = inputTag;
		label.setPos(pos + labelSizeOffset);
	}
}

void TextFieldModifier::setSize(Vector2f size) {
	this->size = size;
	refreshTextField();
}

Vector2f TextFieldModifier::getPos() {
	return pos;
}

Vector2f TextFieldModifier::getSize() {
	return size;
}

void TextFieldModifier::setModifier(bool enabled) {
	if (modifierEnabled != enabled) {
		modifierEnabled = enabled;

		Vector2f oldS = size;
		setSize(size + 1.0f);
		setSize(oldS);

		refreshTextField();
	}
}

bool TextFieldModifier::getModifier() {
	return modifierEnabled;
}

void TextFieldModifier::update() {
	TextField::update();
	mouseInModifier = input::getMouseInQuad(modifierPos, modifierSize, inputTag);

	if (active) {
		if (modifierEnabled) {
			if (mouseInModifier) {
				if (input::getMouseDown(0)) {
					accessingModifier = true;
					input::setCursorInputMode(GLFW_CURSOR_DISABLED);
				}
			}

			if (accessingModifier) {
				curIndex = -1;
				curIndexEnd = -1;
				shadowIndex = -1;
			}

			if (input::getMouseUp(0)) {
				accessingModifier = false;
				input::setCursorInputMode(GLFW_CURSOR_NORMAL);
			}
		}
		else {
			accessingModifier = false;
		}
	}
}

void TextFieldModifier::draw() {
	TextField::draw();

	Vector4f tCol = color;
	if (active) {
		if (mouseInModifier) {
			tCol /= 1.4f;
		}
		else {
			tCol /= 1.6f;
		}

		if (input::getKey(GLFW_KEY_ENTER, inputLayer)) {
			tCol /= 1.4f;
		}

		if (accessingModifier) {
			tCol /= 2.0f;
		}
	}
	else {
		if (mouseInModifier) {
			tCol /= 1.2f;
		}
		else {
			tCol /= 1.1f;
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (tCol.w > 0.0f) {
		glColor4f(tCol.x, tCol.y, tCol.z, color.w);
		util::drawQuad(modifierPos, modifierSize);
	}

	glColor4f(label.getColor().x, label.getColor().y, label.getColor().z, label.getColor().w);
	txr->drawString("<|>", modifierPos + modifierSize/2.0f - modifierBound.size/2.0f + modifierBound.offset, modifierScale);

	if (disabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		util::drawQuad(pos, size);
		glDisable(GL_BLEND);
	}

	glDisable(GL_BLEND);
}
