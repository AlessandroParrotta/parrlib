#include "TextFieldInteger.h"

TextFieldInteger::TextFieldInteger()
{
}

TextFieldInteger::TextFieldInteger(TextRendererOld &txr) : TextFieldModifier(txr) {
	//label.setAlignment(Label::Alignment::RIGHT);
}

TextFieldInteger::TextFieldInteger(int val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action) : TextFieldModifier(textScale, txr, pos, size, backgroundColor, textColor, TextField::Constraint::INTEGER, action) {
	setVal(val);
}

TextFieldInteger::TextFieldInteger(int val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor) : TextFieldInteger(val, textScale, txr, pos, size, backgroundColor, textColor, [] {}) {

}

TextFieldInteger::TextFieldInteger(int val, TextRendererOld& txr, std::function<void()> action) : TextFieldInteger(val, 1.f, txr, 0.f, 0.f, vColor4f::gray, vColor4f::black, action){}


TextFieldInteger::~TextFieldInteger(){

}

void TextFieldInteger::refresh() {
	refreshTextField();
}

void TextFieldInteger::setVal(int val) {
	this->val = val;
	removeString(0, text.length());
	insertString(0, std::to_string(val));
}

int TextFieldInteger::getVal() {
	return val;
}

void TextFieldInteger::refreshTextField() {
	TextFieldModifier::refreshTextField();

	val = atoi(text.c_str());
}

void TextFieldInteger::update() {
	if (active) {
		if (input::getKeyDown(GLFW_KEY_ENTER, inputLayer) || (input::getMouseDown(0, inputLayer) && !input::getMouseInQuad(pos, size))) {
			setVal(val);
		}
	}

	TextFieldModifier::update();

	if (active) {
		if (mouseInModifier) {
			if (input::getMouseDown(0, inputLayer)) {
				accessStartVal = val;
				//accessStartPos = input::getMouseOrtho().x;
				accessStartPos = input::getMousePos().x;
				mouseModifierPos = accessStartPos;
			}
		}

		if (accessingModifier) {
			if (input::getMouseDelta().x != 0.0f) {
				//mouseModifierPos += input::getMouseDeltaCoords().x;
				mouseModifierPos += input::getMouseDelta().x;
				float distVal = ((mouseModifierPos - (accessStartPos))*10.0f);
				val = accessStartVal + (int)(pow(std::fabs(distVal), std::fmax(1.0f, std::fabs(distVal / 10.0f)))*(distVal < 0.0f ? -1.0f : 1.0f));
				/*if (input::getMouseDeltaCoords().x*10000.0f) {
					debug::println(input::getMouseDeltaCoords().x*10000.0f);
				}*/

				//debug::rtPrintln((input::getMouseOrtho().x - (modifierPos.x + modifierSize.x / 2.0f))*5.0f);

				setVal(val);
				//refreshTextField();
			}
		}
	}
}
