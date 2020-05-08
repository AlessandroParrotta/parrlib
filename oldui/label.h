#pragma once

#include <limits>

#include "..\Vector2f.h"
#include "..\Vector4f.h"
#include "..\TextRendererOld.h"

#include "UIElement.h"

class Label : public UIElement{
protected:
	std::string text;
	std::string displayText;

	float textScale = 1.0f;
	float maxTextScale = 0.0f;
	float minTextScale = FLT_MAX;

	bool dynamicScale = true;

public:

	TextRendererOld *txr;
	TextRendererOld::StringBound textBound;
	Vector2f textOffset;

	enum Alignment {
		LEFT,
		RIGHT,
		UP,
		DOWN,

		CENTER,

		UPLEFT,
		UPRIGHT,
		DOWNLEFT,
		DOWNRIGHT
	};

protected:
	Alignment align = Alignment::CENTER;
	float alignOffset = 0.5f;

public:
	virtual void refreshTextBounds();

	Label();
	Label(TextRendererOld &txr);
	Label(std::string text, TextRendererOld &txr, Vector2f pos, Vector2f size);
	Label(std::string text, TextRendererOld& txr);
	Label(std::string text, Vector4f color, TextRendererOld &txr, Vector2f pos, Vector2f size);
	Label(std::string text, Vector4f color, float scale, TextRendererOld &txr, Vector2f pos, Vector2f size);
	Label(std::string text, Vector4f color, float scale, TextRendererOld &txr, Vector2f pos);

	~Label();

	virtual void setPos(Vector2f pos);
	virtual void setSize(Vector2f size);
	virtual Vector2f getSize();

	virtual void setText(std::string text);
	virtual std::string getText();

	virtual void setAlignment(Alignment align);
	virtual Alignment getAlignment();

	virtual void setDynamicScale();
	virtual void setStaticScale(float textScale);

	virtual void setTextScale(float textScale);
	virtual float getTextScale();

	virtual void setMaxTextScale(float maxTextScale);
	virtual float getMaxTextScale();

	virtual void setMinTextScale(float minTextScale);
	virtual float getMinTextScale();

	virtual void setAlignmentOffset(float alignOffset);
	virtual float getAlignmentOffset();

	virtual void refresh();
	virtual void resize();

	virtual void draw();
};

