#include "TextFieldDecimal.h"

TextFieldDecimal::TextFieldDecimal()
{
}

TextFieldDecimal::TextFieldDecimal(float val, TextRendererOld& txr, std::function<void()> action) : TextFieldDecimal(val, 1.f, txr, 0.f, 0.f, vColor4f::gray, vColor4f::black, action) {}

TextFieldDecimal::TextFieldDecimal(TextRendererOld &txr) : TextFieldModifier(txr) {
	//label.setAlignment(Label::Alignment::RIGHT);
}

TextFieldDecimal::TextFieldDecimal(float val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action) : TextFieldModifier(textScale, txr, pos, size, backgroundColor, textColor, TextField::Constraint::DECIMAL, action) {
	setVal(val);
}

TextFieldDecimal::TextFieldDecimal(float val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor) : TextFieldDecimal(val, textScale, txr, pos, size, backgroundColor, textColor, [] {}) {

}


TextFieldDecimal::~TextFieldDecimal() {

}

void TextFieldDecimal::refresh() {
	refreshTextField();
}

void TextFieldDecimal::setVal(float val) {
	this->val = val;
	removeString(0, text.length());

	std::string num = std::to_string(val);
	insertString(0, num.substr(0, num.find('.') + 1 + decimalDigits));
}

float TextFieldDecimal::getVal() {
	return val;
}

void TextFieldDecimal::setDecimalDigits(int decimalDigits) {
	if (this->decimalDigits != decimalDigits) {
		this->decimalDigits = decimalDigits;
		setVal(val);
	}
}

int TextFieldDecimal::getDecimalDigits() {
	return decimalDigits;
}

void TextFieldDecimal::refreshTextField() {
	TextFieldModifier::refreshTextField();

	val = atof(text.c_str());
}

void TextFieldDecimal::update() {
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
			//debug::rtPrintln("accessing! "  + std::to_string(input::getMouseDelta().x));
			if (input::getMouseDelta().x != 0.0f) {
				//mouseModifierPos += input::getMouseDeltaCoords().x;
				mouseModifierPos += input::getMouseDelta().x;
				float distVal = ((mouseModifierPos - (accessStartPos)));
				val = accessStartVal + pow(std::fabs(distVal), std::fmax(1.0f, std::fabs(distVal)))*(distVal < 0.0f ? -1.0f : 1.0f);
				debug::rtPrintln(std::to_string(mouseModifierPos) + " " + std::to_string(distVal) + " " + std::to_string(val));
				/*if (input::getMouseDeltaCoords().x*10000.0f) {
					debug::println(input::getMouseDeltaCoords().x*10000.0f);
				}*/

				//debug::rtlog << distVal;

				setVal(val);
				//refreshTextField();
			}
		}
	}
}
