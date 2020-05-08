#pragma once

#include "TextFieldModifier.h"

class TextFieldInteger : public TextFieldModifier {
protected:
	int val = 0;
	int accessStartVal = 0;
	float accessStartPos = 0.0f;
	float mouseModifierPos = 0.0f;

public:
	TextFieldInteger();
	TextFieldInteger(TextRendererOld &txr);
	TextFieldInteger(int val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action);
	TextFieldInteger(int val, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor);
	TextFieldInteger(int val, TextRendererOld& txr, std::function<void()> action);

	~TextFieldInteger();

	void refresh();

	void refreshTextField();

	void setVal(int val);
	int getVal();

	virtual void update();
};

