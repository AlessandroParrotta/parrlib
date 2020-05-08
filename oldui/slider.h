#pragma once

#include "..\Vector2f.h"

#include "UIElement.h"

#include "TextField\TextFieldDecimal.h"

class Slider : public UIElement{
protected:
	float val, minVal, maxVal;

	Vector2f sliderOff = Vector2f(0.01f, 0.01f);
	float sliderLineWidth = 0.2f;
	float sliderSpacing = 0.0f;
	float sliderEnd = 0.6f;

	Vector2f sliderLinePos, sliderLineSize;
	Vector2f sliderPos, sliderSize;

	float sliderCur = 0.0f;
	float sliderLength = 0.05f;

	bool sliderLocked = false;
	float sliderStartLockingMousePos = 0.0f;

public:
	TextFieldDecimal txf;

	bool mouseInSlider = false;

	enum Scaling {
		LINEAR,
		EXPONENTIAL
	};
protected:
	Scaling scaling = Scaling::LINEAR;
	float scalingExpFactor = 2.0f;

public:
	enum ClampMode {
		WHOLE,
		MIN,
		MAX
	};
protected:
	ClampMode clampMode = ClampMode::WHOLE;

public:
	void refreshSlider();

	Slider();
	Slider(float curVal, float minVal, float maxVal, TextRendererOld &txr, Vector4f backGroundColor, Vector4f textColor, Vector2f pos, Vector2f size);

	~Slider();

	void refresh();

	void setPos(Vector2f pos);
	Vector2f getPos();

	void setSize(Vector2f size);
	Vector2f getSize();

	void setVal(float val);
	float getVal();

	void setScalingLinear();
	void setScalingExponential(float expFactor);

	Scaling getScaling();

	void setClampMode(ClampMode clampMode);
	ClampMode getClampMode();

	void update();

	void draw();
};

