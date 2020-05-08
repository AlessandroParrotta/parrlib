//#include "stdafx.h"
#include "TextField.h"

#include "..\..\debug.h"

void TextField::setActive(bool active) {
	this->active = active;
	if (active) {
		input::addActiveLayer(inputTag);
	}
	else {
		input::removeActiveLayer(inputTag);
	}
}

bool TextField::isActive() {
	return active;
}

void TextField::setChangeCallback(std::function<void()> changeCallback) {
	this->changeCallback = changeCallback;
}

std::function<void()> TextField::getChangeCallback() {
	return changeCallback;
}

Vector2f TextField::getVisualOffsetCoordsActual() {
	return distances.size() > 0 ? distances[min(max(visualOffset, 0), distances.size()-1)] : Vector2f(0.0f); //txr->getStringBounds(text.substr(0, visualOffset).c_str(), label.textScale).size;
}

Vector2f TextField::getVisualOffsetCoords() {
	return visualOffsetCoords;
}

//int TextField::getCharacterFitNum(std::string text, Vector2f bound) {
//	while (txr->getStringBounds(text.c_str(), label.textScale).size.x > bound.x) {
//		if (text.length() == 0) {
//			break;
//		}
//		text = text.substr(0, text.length()-1);
//	}
//
//	return text.length();
//}

void TextField::setText(std::string text) {
	if (this->text != text) {
		removeString(0, this->text.length());
		insertString(0, text);
		changeCallback();
	}
}

std::string TextField::getText() {
	return text;
}
void TextField::setCurIndex(int curIndex) {
	if (this->curIndex != curIndex && curIndex >= 0 && curIndex <= text.length()) {
		this->curIndex = curIndex;
		this->curIndexEnd = curIndex;
	}
}

int TextField::getCurIndex() {
	return curIndex;
}

void TextField::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
		inputLayer = inputTag;
		label.setPos(pos + labelSizeOffset);
	}
}

void TextField::setSize(Vector2f size) {
	this->size = size;

	resize();

	label.setSize(size - (labelSizeOffset*2.0f));
	refreshTextField();
}

Vector2f TextField::getPos() {
	return pos;
}

Vector2f TextField::getSize() {
	return size;
}

void  TextField::setTextScale(float textScale) {
	if (this->textScale != textScale) {
		this->textScale = textScale;
	}
}

float  TextField::getTextScale() {
	return textScale;
}

int TextField::getCharacterFitNum(int offset, Vector2f bound) {
	int cNum = 0;

	if (distances.size() > 0) {
		int iter = offset;
		while (iter < distances.size() && cNum < text.length()) {
			if (distances[iter] - distances[offset] >= bound.x) {
				break;
			}
			
			cNum = iter - offset;

			iter++;
		}
	}

	return cNum;
}

void TextField::refreshTextField() {
	float oldTextScale = label.getTextScale();
	label.setTextScale(label.txr->getScreenPercScaleFactor(label.getSize().y*textScale));
	label.refresh();

	if (oldTextScale != label.getTextScale()) {
		std::string tText = text;
		setText("");
		setText(tText);
	}

	if(visualOffset <= text.length()) {
		sizeInCharacters = getCharacterFitNum(visualOffset, label.getSize());//getCharacterFitNum(text.substr(visualOffset, text.length()), label.getSize());

		int oldVisualOffset = visualOffset;
		while (visualOffset >= 0 && visualOffset + sizeInCharacters == text.length()) {
			visualOffset--;

			sizeInCharacters = getCharacterFitNum(visualOffset, label.getSize());
		}
		if (oldVisualOffset != visualOffset) {
			visualOffset++;
			sizeInCharacters = getCharacterFitNum(visualOffset, label.getSize());
		}

		label.setText(text.substr(visualOffset, sizeInCharacters));
	}

	/*distances.clear();
	for (int i = 0; i < text.length(); i++) {
		distances.push_back(txr->getStringBounds(text.substr(0, i).c_str(), label.textScale).size.x);
	}
	distances.push_back(txr->getStringBounds(text.c_str(), label.textScale).size.x);*/

	textBarSize = (label.txr->fontSize*label.getTextScale()/(float)cst::resx())*0.2f;

	visualOffsetCoords = getVisualOffsetCoordsActual();
}

void TextField::resize() {
	std::string tText = text;
	setText("");
	setText(tText);
}

TextField::TextField(){
	//barSizeReference = (float)constants::WIDTH;
}

TextField::TextField(TextRendererOld &txr) : TextField() {
	this->txr = &txr;
}

TextField::TextField(std::string text, TextRendererOld& txr, std::function<void()> action) : TextField(text, 1.f, txr, 0.f,0.f, vColor4f::gray, vColor4f::black, action) {}

TextField::TextField(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor) : TextField(txr){
	label = Label(text, textColor, 1.0f, txr, pos + labelSizeOffset, size - (labelSizeOffset*2.0f));
	label.setAlignment(Label::Alignment::LEFT);
	setTextScale(textScale);
	this->text = text;
	setPos(pos);
	setSize(size);
	this->color = backgroundColor;

	distances.clear();
	for (int i = 0; i < text.length(); i++) {
		distances.push_back(txr.getStringBounds(text.substr(0, i), label.getTextScale()).size.x);
	}
	distances.push_back(txr.getStringBounds(text, label.getTextScale()).size.x);

	refreshTextField();
	inputLayer = inputTag;
	action = [] {};
}

TextField::TextField(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action) : TextField(text, textScale, txr, pos, size, backgroundColor, textColor) {
	this->action = action;
}

TextField::TextField(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, Constraint constraint, std::function<void()> action) : TextField(text, textScale, txr, pos, size, backgroundColor, textColor, action) {
	this->constraint = constraint;
}

TextField::~TextField(){

}

void TextField::refresh() {
	refreshTextField();
}

void TextField::insertString(int index, std::string s) {
	text = text.substr(0, index) + s + text.substr(index, text.length());
	
	//debug::println("inserting " + s + "!");

	distances.erase(distances.begin() + index, distances.end());

	std::vector<float> updatedDists(text.length() - index);
	for (int i = 0; i < text.length() - index; i++) {
		updatedDists[i] = txr->getStringBounds(text.substr(0, index + i), label.getTextScale()).size.x;
	}
	updatedDists.push_back(txr->getStringBounds(text, label.getTextScale()).size.x);

	distances.insert(distances.begin() + index, updatedDists.begin(), updatedDists.end());

	refreshTextField();
}

void TextField::removeString(int from, int to) {
	if (from < to) {
		text = text.substr(0, from) + text.substr(to, text.length()-to);

		distances.erase(distances.begin() + from, distances.end());

		std::vector<float> updatedDists(text.length() - from);
		for (int i = 0; i < text.length() - from; i++) {
			updatedDists[i] = txr->getStringBounds(text.substr(0, from + i), label.getTextScale()).size.x;
		}
		updatedDists.push_back(txr->getStringBounds(text, label.getTextScale()).size.x);

		distances.insert(distances.begin() + from, updatedDists.begin(), updatedDists.end());

		refreshTextField();
	}
}

bool TextField::checkConstraints(std::string s) {
	bool check = true;

	switch (constraint) {
	case NONE:
		return true;
	case INTEGER:
		check = true;

		for (int i = 0; i < s.length() && check; i++) {
			check = isdigit(s[i]) || s[i] == '-';
		}

		return check;
	case DECIMAL:
		check = true;

		bool alreadyDot = false;
		for (int i = 0; i < text.length() &&  !alreadyDot; i++) {
			alreadyDot = text[i] == '.';
		}

		for (int i = 0; i < s.length() && check; i++) {
			if (alreadyDot && s[i] == '.') {
				return false;
			}

			alreadyDot = s[i] == '.';

			check = isdigit(s[i]) || s[i] == '.' || s[i] == '-';
		}

		return check;
	}

	return true;
}

void TextField::update() {
	UIElement::update();

	mouseInLabel = input::getMouseInQuad(label.getPos(), label.getSize(), inputTag);

	if (input::getMouseDown(0)) {
		if (mouseInside) {
			//active = true;
			//input::addActiveLayer(inputTag);
			setActive(true);
		}
		else {
			if (active) {
				setActive(false);
				action();
			}
			active = false;
		}
	}

	if (active) {
		//debug::rtPrintln(curIndex);
		

		if (input::getKeyDown(GLFW_KEY_ENTER, inputLayer)) {
			action();
		}

		if (mouseInLabel) {
			if (input::getMouseDown(0)) {
				selectingString = true;
			}

			Vector2f visualCoords = Vector2f(getVisualOffsetCoords().x, 0.0f);
			int tIndex = -1;
			float dist = 1e09;
			for (int i = visualOffset; i < util::clamp(visualOffset + sizeInCharacters+1, 0, distances.size()); i++) {
				//float tDist = std::fabs(input::getMouseOrtho().x - (label.getPos().x - visualCoords.x + label.textOffset.x + distances[i]));
				float tDist = std::fabs(input::getMousePos().x - (label.getPos().x - visualCoords.x + label.textOffset.x + distances[i]));
				if (tDist < dist) {
					tIndex = i;
					dist = tDist;
				}
			}

			shadowIndex = tIndex;

			if (input::getMouseDown(0)) {
				curIndex = shadowIndex;
				curIndexEnd = shadowIndex;
			}

			//Vector2f mousePos = input::getMouseOrtho();
			

			bool justClicked = false;
			if (oldIndex == shadowIndex) {
				if (input::getMouseDown(0)) {
					timesClicked++;
					justClicked = true;
					//timesClicked %= 4;
				}

				if (timesClicked > 0) {
					clickTimer += gtime::deltaTime;

					if (clickTimer > 1.3f) {
						clickTimer = 0.0f;
						timesClicked = 0;
					}
				}
			}
			else {
				timesClicked = 0;
			}

			if(timesClicked < 1){
				if (selectingString) {
					curIndexEnd = shadowIndex;

					int oldVisualOffset = visualOffset;
					int endOffset = visualOffset + sizeInCharacters;
					if (endOffset == shadowIndex && endOffset != text.length()) {
						visualOffset++;
					}
					if (visualOffset == shadowIndex && visualOffset != 0) {
						visualOffset--;
					}

					if (oldVisualOffset != visualOffset) {
						refreshTextField();
					}

					/*if (curIndex > curIndexEnd) {
						int t = curIndex;
						curIndex = curIndexEnd;
						curIndexEnd = t;
					}*/
				}
			}

			if (timesClicked == 2 && justClicked) {
				bool firstAlNum = false, whitespace = text[curIndexEnd] == ' ' || curIndexEnd == distances.size()-1;

				if (!isalnum(text[curIndexEnd])) {
					if (!whitespace) {
						curIndexEnd = min(curIndexEnd + 1, distances.size() - 1);
					}
				}
				else {
					while (isalnum(text[curIndexEnd])) {
						if (curIndexEnd >= distances.size() - 1) {
							break;
						}

						if (!firstAlNum) {
							firstAlNum = isalnum(text[curIndexEnd]);
						}

						curIndexEnd++;
					}
				}

				if (firstAlNum || whitespace) {
					if (!isalnum(text[max(0, curIndex - 1)])) {
						if (!firstAlNum && text[max(0, curIndex - 1)] != ' ') {
							curIndex = max(curIndex - 1, 0);
						}
					}
					else {
						while (isalnum(text[max(0, curIndex - 1)])) {
							if (curIndex <= 0) {
								break;
							}


							curIndex--;
						}
					}
				}
			}
			else if (timesClicked == 3 && justClicked) {
				curIndex = 0;
				curIndexEnd = distances.size() - 1;
				
				//timesClicked = 0;
				//debug::println(curIndexEnd);
			}

			oldIndex = shadowIndex;
			//oldMouse = mousePos;
		}
		else {
			shadowIndex = -1;
		}

		if (input::getMouseUp(0)) {
			selectingString = false;
		}

		if (input::getKey(GLFW_KEY_LEFT_CONTROL, inputLayer)) {
			if (input::getKeyDown(GLFW_KEY_C, inputLayer)) {
				if (curIndex != curIndexEnd) {
					if (curIndex > curIndexEnd) {
						int t = curIndex;
						curIndex = curIndexEnd;
						curIndexEnd = t;
					}

					//debug::println("copying " + text.substr(curIndex, curIndexEnd - curIndex));
					input::copyToClipboard(text.substr(curIndex, curIndexEnd - curIndex));
				}
				else {
					input::copyToClipboard(text);
				}
			}

			if (input::getKeyDown(GLFW_KEY_V, inputLayer)) {
				if (curIndex != curIndexEnd) {
					if (curIndex > curIndexEnd) {
						int t = curIndex;
						curIndex = curIndexEnd;
						curIndexEnd = t;
					}

					removeString(curIndex, curIndexEnd);
				}
				
				std::string fromClipboard = input::getFromClipboard();
				if (checkConstraints(fromClipboard)) {
					insertString(curIndex, fromClipboard);
					curIndex += fromClipboard.length();
					curIndexEnd = curIndex;
					changeCallback();
				}
			}
		}
		else {
			if (input::getKeyDown(GLFW_KEY_LEFT, inputLayer)) {
				curIndex--;
				curIndexEnd = curIndex;
			}
			if (input::getKeyDown(GLFW_KEY_RIGHT, inputLayer)) {
				curIndex++;
				curIndexEnd = curIndex;
			}

			if (input::getKeyDown(GLFW_KEY_BACKSPACE, inputLayer)) {
				if (curIndex == curIndexEnd) {
					if (curIndex > 0) {
						removeString(curIndex - 1, curIndex);
						curIndex--;
						//visualOffset--;
					}
				}
				else {
					if (curIndex > curIndexEnd) {
						int t = curIndex;
						curIndex = curIndexEnd;
						curIndexEnd = t;
					}

					removeString(curIndex, curIndexEnd);
					//visualOffset -= (curIndexEnd - curIndex);
				}
				curIndexEnd = curIndex;
				refreshTextField();
				changeCallback();
			}

			if (input::getKeyDown(GLFW_KEY_DELETE, inputLayer)) {
				if (curIndex == curIndexEnd) {
					if (curIndex < text.length()) {
						removeString(curIndex, curIndex + 1);
					}
				}
				else {
					if (curIndex > curIndexEnd) {
						int t = curIndex;
						curIndex = curIndexEnd;
						curIndexEnd = t;
					}

					removeString(curIndex, curIndexEnd);
				}
				curIndexEnd = curIndex;
				changeCallback();
			}

			unsigned int anyKeyt = input::pollTextKey();
			if (anyKeyt > 0) {

				std::wstring_convert<std::codecvt_utf8<unsigned int>, unsigned int> converter;
				std::string u8str = converter.to_bytes(anyKeyt);

				/*std::cout << "u8str: \"" << u8str << "\"(";
				for (int i = 0; i < u8str.length(); i++) {
					std::cout << u8str[i] << " (" << (int)u8str[i] << "), ";
				}
				std::cout << "\b\b)\n";*/
				
				if (checkConstraints(u8str)) {
					if (curIndex != curIndexEnd) {
						if (curIndex > curIndexEnd) {
							int t = curIndex;
							curIndex = curIndexEnd;
							curIndexEnd = t;
						}

						removeString(curIndex, curIndexEnd);
					}

					insertString(curIndex, u8str);

					curIndex += u8str.length();
					curIndexEnd = curIndex;

					changeCallback();
				}
			}
		}

		curIndex = util::clamp(curIndex, 0, distances.size()-1);
		curIndexEnd = util::clamp(curIndexEnd, 0, distances.size() - 1);

		if (!input::getMouse(0) && curIndex == curIndexEnd) {
			int oldVisualOffset = visualOffset;
			int endOffset = visualOffset + sizeInCharacters;
			while (endOffset < curIndexEnd) {
				visualOffset++;
				endOffset = visualOffset + sizeInCharacters;
			}
			while (visualOffset >= curIndex) {
				visualOffset--;
			}


			visualOffset = util::clamp(visualOffset, 0, distances.size() - 1);

			if (oldVisualOffset != visualOffset) {
				refreshTextField();
			}
		}
	}
}

Vector2f TextField::getBarPos(int index) {
	Vector2f visualCoords = Vector2f(getVisualOffsetCoords().x, 0.0f);
	
	//Vector2f barPos =  vec2(label.getPos().x, pos.y) - visualCoords - vec2(label.textBound.offset.x,0.f) + vec2(label.textOffset.x,0.f) + vec2(distances[index], 0.0f); //- label.textBound.offset + label.textOffset + vec2(distances[index], 0.0f);
	Vector2f barPos = label.getPos() - visualCoords - label.textBound.offset + label.textOffset + vec2(distances[index], 0.0f);

	//float labelPos = label.pos.x - label.textBound.offset.x + label.textOffset.x;
	//return Vector2f(std::fmin(std::fmax(barPos.x, labelPos), labelPos + distances[visualOffset + sizeInCharacters]), barPos.y);
	return Vector2f(std::fmin(std::fmax(barPos.x, label.getPos().x), label.getPos().x + label.getSize().x), barPos.y);
}

void TextField::drawTextBar(int index, Vector4f color) {
	if (index >= 0 && index < distances.size()) {
		glColor4f(color.x, color.y, color.z, color.w);
		util::drawQuad(getBarPos(index), Vector2f(textBarSize, label.textBound.size.y));
		//util::drawQuad(getBarPos(index), Vector2f(textBarSize, size.y));
	}
}

void TextField::draw() {
	drawBackground(this);

	if (active) {
		if (curIndex != curIndexEnd) {
			glColor4f(0.4f, 0.6f, 0.6f, 1.0f);
			Vector2f curIndexPos = getBarPos(curIndex), curIndexEndPos = getBarPos(curIndexEnd);
			util::drawQuad(curIndexPos, Vector2f(curIndexEndPos.x - curIndexPos.x, label.textBound.size.y));
		}

		drawTextBar(shadowIndex, Vector4f(0.8f, 0.8f, 0.8f, 1.0f));
		drawTextBar(curIndex, Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	}

	label.draw();

	glDisable(GL_BLEND);

	if (disabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		util::drawQuad(pos, size);
		glDisable(GL_BLEND);
	}
}
