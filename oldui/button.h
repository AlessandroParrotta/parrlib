#pragma once

#include <functional>

#include "..\Vector2f.h"
#include "..\Vector4f.h"
#include "..\Input.h"
#include "..\TextRendererOld.h"

#include "UIElement.h"

#include "Label.h"

class Button : public UIElement{
protected:
	float textScale = 0.8f;

public:
	Vector2f texCoord0, texCoord1;

	Label label;

	bool locked = false;

	Button();
	Button(TextRendererOld &txr);
	Button(std::string text, TextRendererOld& txr, std::function<void()> action);
	Button(std::string text, TextRendererOld &txr, Vector2f pos,Vector2f size);
	Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size, std::function<void()> action);
	Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action);
	Button(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action);
	Button(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, Vector2f texCoord0, Vector2f texCoord1, std::function<void()> action);

	~Button();

	void refresh();

	std::function<void()> action;

	virtual void setPos(Vector2f pos);

	virtual void setSize(Vector2f size);
	Vector2f getSize();

	virtual void setTextScale(float textScale);
	virtual float getTextScale();

	virtual void update();

	virtual void draw();
	virtual void draw(int &current, int &colCurrent, float *posBuf, float *colBuf, float *texBuf, TextRendererOld txr);
};

