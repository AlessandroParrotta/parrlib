#pragma once

#include "..\..\Vector2f.h"
#include "..\..\TextRendererOld.h"
#include "..\..\debug.h"

#include "TextField.h"


class TextFieldModifier : public TextField {
protected:
	float modifierOffset = 0.8f;
	float modifierSpacing = 0.02f;
	Vector2f modifierPos;
	Vector2f modifierSize;

	float modifierScale;
	TextRendererOld::StringBound modifierBound;

	bool modifierEnabled = true;

public:
	bool mouseInModifier = false;

	bool accessingModifier = false;

	TextFieldModifier();
	TextFieldModifier(TextRendererOld &txr);
	TextFieldModifier(float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, TextField::Constraint constraint, std::function<void()> action);
	TextFieldModifier(float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, TextField::Constraint constraint);

	~TextFieldModifier();

	void refresh();

	virtual void refreshTextField();

	void setPos(Vector2f pos);
	void setSize(Vector2f size);

	Vector2f getPos();
	Vector2f getSize();

	void setModifier(bool enabled);
	bool getModifier();

	virtual void update();

	void draw();
};

