#pragma once

#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <cassert>
#include <functional>

#include "..\..\Vector2f.h"

#include "../UIElement.h"

#include "../Label.h"

class TextField : public UIElement{
protected:
	Vector2f labelSizeOffset = Vector2f(0.01f, 0.01f);
	std::function<void()> changeCallback = [] {};

	std::string text;
	int curIndex = -1;
	int curIndexEnd = -1;
	int shadowIndex = -1;
	int visualOffset = 0;
	Vector2f visualOffsetCoords;

	float textScale = 0.55f;

	bool active = false;

public:
	Label label;
	TextRendererOld *txr;

	bool mouseInLabel = false;

	enum Constraint {
		NONE,
		INTEGER,
		DECIMAL
	};
	Constraint constraint = Constraint::NONE;
	
	bool selectingString = false;

	int sizeInCharacters = 0;

	float clickTimer = 0.0f;
	int timesClicked = 0;
	//Vector2f oldMouse;
	int oldIndex;

	std::function<void()> action;

	void setChangeCallback(std::function<void()> changeCallback);
	std::function<void()> getChangeCallback();

	Vector2f getVisualOffsetCoordsActual();
	Vector2f getVisualOffsetCoords();

	std::vector<float> distances;

	void setText(std::string text);
	std::string getText();

	void setCurIndex(int curIndex);
	int getCurIndex();

	void setActive(bool active);
	bool isActive();

	virtual void setPos(Vector2f pos);
	virtual void setSize(Vector2f size);

	virtual Vector2f getPos();
	virtual Vector2f getSize();

	virtual void setTextScale(float textScale);
	virtual float getTextScale();

	//int getCharacterFitNum(std::string text, Vector2f bound);
	int getCharacterFitNum(int offset, Vector2f bound);
	virtual void refreshTextField();

	void resize();

	TextField();
	TextField(TextRendererOld &txr);
	TextField(std::string text, TextRendererOld& txr, std::function<void()> action);
	TextField(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor);
	TextField(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, std::function<void()> action);
	TextField(std::string text, float textScale, TextRendererOld &txr, Vector2f pos, Vector2f size, Vector4f backgroundColor, Vector4f textColor, Constraint constraint, std::function<void()> action);

	~TextField();

	void refresh();

	void insertString(int index, std::string s);
	void removeString(int from, int to);

	bool checkConstraints(std::string s);

	virtual void update();

	Vector2f getBarPos(int index);

	float textBarSize = 0.005f;
	void drawTextBar(int index, Vector4f color);

	virtual void draw();
};

