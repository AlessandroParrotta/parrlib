#include "Label.h"

void Label::refreshTextBounds() {
	if (!txr) return;

	displayText = text;
	if (dynamicScale) {
		textScale = txr->getScaleToFit(displayText, size);// -size * 0.1f);
		if (maxTextScale != 0.0f) textScale = min(textScale, maxTextScale);
		if (minTextScale < maxTextScale && textScale < minTextScale) textScale = minTextScale;

		float screenPercHeight = txr->fontSize*textScale;
		if (screenPercHeight < constants::textSizeThreshold || txr->fontSize*textScale < 7.0f) {

			displayText = displayText + "...";
			while (screenPercHeight < constants::textSizeThreshold || txr->fontSize*textScale < 7.0f) {
				if (displayText.compare("...") == 0) {
					break;
				}

				displayText = displayText.substr(0, displayText.length() - 4);
				displayText = displayText + "...";

				textScale = txr->getScaleToFit(displayText, size - size * 0.1f);
				if (maxTextScale != 0.0f) textScale = min(textScale, maxTextScale);
				if (minTextScale < maxTextScale && textScale < minTextScale) textScale = minTextScale;

				screenPercHeight = txr->fontSize*textScale / (float)cst::resy();
			}
		}

		textScale = txr->getScaleToFit(displayText, size);// -size * 0.1f);
		if (maxTextScale != 0.0f) textScale = min(textScale, maxTextScale);
		if (minTextScale < maxTextScale && textScale < minTextScale) textScale = minTextScale;
	}
	else {
		TextRendererOld::StringBound tBound = txr->getStringBounds(displayText, textScale);

		if (tBound.size.y > size.y) {
			displayText = "...";
		}
		else if(tBound.size.x > size.x){
			displayText += "...";
			while (tBound.size.x > size.x) {
				if (displayText.compare("...") == 0) {
					break;
				}

				displayText = displayText.substr(0, displayText.length() - 4);
				displayText = displayText + "...";
				tBound = txr->getStringBounds(displayText, textScale);
			}
		}
	}

	textBound = txr->getStringBounds(displayText, textScale);
	if (align == Alignment::CENTER) {
		textOffset.x = size.x / 2.0f - textBound.size.x / 2.0f + textBound.offset.x;
		textOffset.y = size.y / 2.0f - textBound.size.y / 2.0f + textBound.offset.y;
	}
	else if (align == Alignment::LEFT) {
		textOffset.x = 0.0f + textBound.offset.x;
		textOffset.y = size.y * alignOffset - textBound.size.y*alignOffset + textBound.offset.y;
	}
	else if (align == Alignment::RIGHT) {
		textOffset.x = size.x - textBound.size.x + textBound.offset.x;
		textOffset.y = size.y * alignOffset - textBound.size.y*alignOffset + textBound.offset.y;
	}
	else if (align == Alignment::UP) {
		textOffset.x = size.x * alignOffset - textBound.size.x*alignOffset + textBound.offset.x;
		textOffset.y = size.y - textBound.size.y + textBound.offset.y;
	}
	else if (align == Alignment::DOWN) {
		textOffset.x = size.x * alignOffset - textBound.size.x*alignOffset + textBound.offset.x;
		textOffset.y = 0.0f + textBound.offset.y;
	}
	else if (align == Alignment::UPLEFT) {
		textOffset.x = 0.0f + textBound.offset.x;
		textOffset.y = size.y - textBound.size.y + textBound.offset.y;
	}
	else if (align == Alignment::UPRIGHT) {
		textOffset.x = size.x - textBound.size.x + textBound.offset.x;
		textOffset.y = size.y - textBound.size.y + textBound.offset.y;
	}
	else if (align == Alignment::DOWNLEFT) {
		textOffset.x = 0.0f + textBound.offset.x;
		textOffset.y = 0.0f + textBound.offset.y;
	}
	else if (align == Alignment::DOWNRIGHT) {
		textOffset.x = size.x - textBound.size.x + textBound.offset.x;
		textOffset.y = 0.0f + textBound.offset.y;
	}
}

void Label::refresh() {
	UIElement::refresh();
	refreshTextBounds();
}

void Label::resize() {
	refresh();
}

Label::Label(){
	color = Vector4f(0.0f,0.0f,0.0f,1.0f);
	backgroundColor = 0.f;
}

Label::Label(TextRendererOld &txr) : Label(){
	this->txr = &txr;
}

Label::Label(std::string text, TextRendererOld& txr) : Label(txr) {
	this->text = text;
}

Label::Label(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size) : Label(txr) {
	this->text = text;
	this->pos = pos;
	this->size = size;
	refreshTextBounds();
}

Label::Label(std::string text, Vector4f color, TextRendererOld &txr, Vector2f pos, Vector2f size) : Label(txr) {
	this->text = text;
	this->color = color;
	this->pos = pos;
	this->size = size;
	textScale = txr.getScreenPercScaleFactor(txr.getScaleToFit(text, size));
	refreshTextBounds();
}

Label::Label(std::string text, Vector4f color, float scale, TextRendererOld &txr, Vector2f pos, Vector2f size) : Label(txr) {
	this->text = text;
	this->color = color;
	this->pos = pos;
	this->size = size;
	this->textScale = scale;
	dynamicScale = false;
	//setMaxTextScale(scale);
	refreshTextBounds();
}

Label::Label(std::string text, Vector4f color, float scale, TextRendererOld &txr, Vector2f pos) : Label(txr) {
	this->text = text;
	this->color = color;
	this->pos = pos;
	this->size = txr.getStringBounds(text, scale).size;
	this->textScale = scale;
	dynamicScale = false;
	refreshTextBounds();
}

Label::~Label()
{
}

void Label::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
	}
}

void Label::setSize(Vector2f size) {
	this->size = size;
	refreshTextBounds();
}

Vector2f Label::getSize() {
	return size;
}

void Label::setText(std::string text) {
	if (text.compare(this->text) != 0) {
		this->text = text;
		refreshTextBounds();
	}
}

std::string Label::getText() {
	return text;
}

void Label::setAlignment(Alignment align) {
	if (this->align != align) {
		this->align = align;
		refreshTextBounds();
	}
}

Label::Alignment Label::getAlignment() {
	return align;
}

void Label::setDynamicScale() {
	if (!dynamicScale) {
		dynamicScale = true;
		refreshTextBounds();
	}
}

void Label::setStaticScale(float textScale) {
	if (dynamicScale) {
		dynamicScale = false;

		if (maxTextScale != 0.0f) textScale = min(textScale, maxTextScale);
		if (minTextScale < maxTextScale && textScale < minTextScale) textScale = minTextScale;
		this->textScale = textScale;

		refreshTextBounds();
	}
}

void Label::setTextScale(float textScale) {
	if (maxTextScale != 0.0f) textScale = min(textScale, maxTextScale);
	if (minTextScale < maxTextScale && textScale < minTextScale) textScale = minTextScale;

	if (dynamicScale) {
		setStaticScale(textScale);
	}
	else {
		if (this->textScale != textScale) {
			this->textScale = textScale;
			refreshTextBounds();
		}
	}
}

float Label::getTextScale() {
	return textScale;
}

void Label::setMaxTextScale(float maxTextScale) {
	if (maxTextScale != this->maxTextScale) {
		this->maxTextScale = maxTextScale;
		if (textScale > maxTextScale) {
			//setTextScale(maxTextScale);
			textScale = maxTextScale;
			refreshTextBounds();
		}
	}
}

float Label::getMaxTextScale() {
	return maxTextScale;
}

void Label::setMinTextScale(float minTextScale) {
	if (minTextScale != this->minTextScale) {
		this->minTextScale = minTextScale;
		if (textScale < minTextScale) {
			//setTextScale(minTextScale);
			textScale = minTextScale;
			refreshTextBounds();
		}
	}
}

float Label::getMinTextScale() {
	return minTextScale;
}

void Label::setAlignmentOffset(float alignOffset) {
	if (this->alignOffset != alignOffset) {
		this->alignOffset = alignOffset;
		refreshTextBounds();
	}
}

float Label::getAlignmentOffset() {
	return alignOffset;
}

void Label::draw() {
	if (!txr) return;

	txr->color = color;

	/*TextRendererOld::StringBound b = txr->getStringBounds(displayText, textScale);
	glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
	util::drawQuad(pos + textOffset - b.offset, b.size);*/
	UIElement::drawBackground(this);
	txr->drawString(displayText, pos + textOffset, textScale);
}
