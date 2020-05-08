#pragma once

#include <functional>

#include "Button.h"

class CheckBox : public UIElement {
public:
	Vector2f offSize = Vector2f(0.1f);
	bool checked;

	bool locked = false;

	CheckBox();
	CheckBox(std::function<void()> action);
	CheckBox(Vector2f pos, Vector2f size, Vector4f color);

	~CheckBox();

	std::function<void()> action = [] {};

	void update();

	void draw();
};

