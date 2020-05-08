#pragma once

#include "TextFieldModifier.h"

class TextFieldDecimal : public TextFieldModifier {
protected:
	float val = 0.0f;
	float accessStartVal = 0.0f;
	float accessStartPos = 0.0f;
	float mouseModifierPos = 0.0f;

	int decimalDigits = 6;

public:
	TextFieldDecimal();
	TextFieldDecimal(TextRendererOld &txr);
	TextFieldDecimal(float val, TextRendererOld& txr, std::function<void()> action);
	TextFieldDecimal(float val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action);
	TextFieldDecimal(float val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor);

	~TextFieldDecimal();

	void refresh();

	void refreshTextField();

	void setVal(float val);
	float getVal();

	void setDecimalDigits(int decimalDigits);
	int getDecimalDigits();

	virtual void update();
};

