#pragma once
#include <vector>

#include "..\Vector2f.h"

#include "UIElement.h"

class UIContainer : public UIElement {
	struct Slider {
		Vector2f boundPos, boundSize;
		Vector2f pos, size;
		Vector4f color;
		bool mouseInside = false;
		bool locked = false;
		Vector2f lockedPos;

		Vector2f axis;

		Slider() {

		}

		Slider(Vector2f pos, Vector2f size, Vector2f boundPos, Vector2f boundSize, Vector4f color) {
			this->pos = pos;
			this->size = size;
			this->boundPos = boundPos;
			this-> boundSize = boundSize;
			this->color = color;
		}

		void setPos(float perc) {
			if (boundSize.x >= boundSize.y) {
				axis.x = 1.0f;
			}
			else {
				axis.y = 1.0f;
			}

			pos = boundPos + boundSize * axis*perc - size * axis*perc;
		}

		void update() {
			if (boundSize.x >= boundSize.y) {
				axis.x = 1.0f;
			}
			else {
				axis.y = 1.0f;
			}

			mouseInside = input::getMouseInQuad(pos, size);
			if (mouseInside) {
				if (input::getMouseDown(0)) {
					locked = true;
					//Vector2f m = input::getMouseOrtho();
					Vector2f m = input::getMousePos();
					Vector2f defPos = pos + size*((pos-boundPos)/boundSize);
					lockedPos = Vector2f(m.x-(m.x-defPos.x), m.y + boundSize.y);
					debug::println(m.toString());
					debug::println(defPos.toString());
					debug::println(lockedPos.toString());
					//lockedPos = input::getMouseOrtho() - (input::getMouseOrtho() - (pos.x + size.x / 2.0f));
				}
			}

			if (locked) {
				if (input::getMouseUp(0)) {
					locked = false;
				}
			}
		}

		void draw() {
			glColor4f(color.x, color.y, color.z, color.w);
			util::drawQuad(boundPos, boundSize);

			Vector4f tCol = color/1.2f;
			if (mouseInside) {
				if (locked) {
					tCol /= 2.0f;
				}
				else {
					tCol /= 1.5f;
				}
			}

			glColor4f(tCol.x, tCol.y, tCol.z, tCol.w);
			util::drawQuad(pos, size);
		}
	};
protected:
	Vector2f slider = 0.0f;
	Vector2f sliderPerc = 0.0f;

	Vector2f sliderSize = Vector2f(0.1f,0.05f);

	Slider sliderX, sliderY;

	Vector2f elemBoundSize;

public:
	bool oldMouseInside = false;
	bool allUnfocused = false;

	bool allContainedX = true;
	bool allContainedY = true;

	struct CardinalAttribute {
		float top, bottom, left, right;

		CardinalAttribute() {

		}

		CardinalAttribute(float val) {
			top = bottom = left = right = val;
		}

		CardinalAttribute(float bottom, float left, float top, float right) {
			this->top = top;
			this->bottom = bottom;
			this->left = left;
			this->right = right;
		}

	};

	struct Properties {
		CardinalAttribute margin, padding;

		Properties() {

		}

		Properties(CardinalAttribute margin, CardinalAttribute padding){
			this->margin = margin;
			this->padding = padding;
		}
	};

	struct Element {
		UIElement* el;
		Properties properties;

		Element() {

		}

		Element(UIElement* el, Properties properties) {
			this->el = el;
			this->properties = properties;
		}
	};
protected:
	std::vector<Element*> elements;

public:

	enum Layout {
		FREE,
		VERTICAL,
		HORIZONTAL,
		ORDERED_FIT,
		UNORDERED_FIT
	};
protected:
	Layout layout = Layout::FREE;

public:

	UIContainer();
	UIContainer(std::vector<Element*> elements, Vector2f pos, Vector2f size);
	UIContainer(std::vector<Element*> elements, Vector2f pos, Vector2f size, Vector4f color);
	
	~UIContainer();

	void addElem(int i, Element* elem);
	void addElem(Element* elem);
	void addElem(int i, UIElement* uiElem);
	void addElem(UIElement* uiElem);

	void removeElem(int i);

	Element* getElem(int i);

	void refitElements();

	void setLayout(Layout layout);
	Layout getLayout();

	void resize();

	void refresh();

	void setPos(Vector2f pos);
	void setSize(Vector2f size);

	void update();

	void draw();
	
	void dispose();
};

