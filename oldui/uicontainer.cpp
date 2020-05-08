#include "UIContainer.h"



UIContainer::UIContainer()
{
}

UIContainer::UIContainer(std::vector<Element*> elements, Vector2f pos, Vector2f size, Vector4f color) : UIElement(pos, size, color) {
	this->elements = elements;

	sliderX = Slider(Vector2f(0.0f, 0.0f), sliderSize, Vector2f(0.0f), Vector2f(0.0f), color*1.3f);
	sliderY = Slider(Vector2f(0.0f, 0.0f), Vector2f(sliderSize.y, sliderSize.x), Vector2f(0.0f), Vector2f(0.0f), color*1.3f);

	refitElements();
	refresh();
}

UIContainer::UIContainer(std::vector<Element*> elements,Vector2f pos, Vector2f size) : UIContainer(elements, pos, size, Vector4f(0.7f,0.7f,0.7f,1.0f)) {

}


UIContainer::~UIContainer()
{
}

void UIContainer::addElem(int i, Element* elem) {
	if (i == elements.size() - 1) {
		elements.push_back(elem);
	}
}

void UIContainer::addElem(Element* elem) {
	addElem(elements.size() - 1, elem);
}

void UIContainer::addElem(int i, UIElement* uiElem) {
	addElem(i, new Element(uiElem, Properties(CardinalAttribute(0.05f), CardinalAttribute(0.0f))));
}

void UIContainer::addElem(UIElement* uiElem) {
	addElem(elements.size()-1, uiElem);
}

void UIContainer::removeElem(int i) {
	elements[i]->el->dispose();
	delete elements[i]->el;
	delete elements[i];
	elements.erase(elements.begin() + i);
}

UIContainer::Element* UIContainer::getElem(int i) {
	return elements[i];
}

void UIContainer::refitElements() {
	Vector2f advancePos = 0.0f;
	int prevLine = 0;

	switch (layout) {
	case FREE:
		break;
	case VERTICAL:
		for (int i = 0; i < elements.size(); i++) {
			
			Vector2f pos = advancePos + Vector2f(elements[i]->properties.margin.left, -elements[i]->properties.margin.top - elements[i]->el->getSize().y);
			elements[i]->el->setPos(this->pos + Vector2f(0.0f, size.y) + pos);
			elements[i]->el->refresh();
			advancePos.y -= elements[i]->properties.margin.top + elements[i]->el->getSize().y + elements[i]->properties.margin.bottom;
		}
		break;
	case HORIZONTAL:
		for (int i = 0; i < elements.size(); i++) {

			Vector2f pos = advancePos + Vector2f(elements[i]->properties.margin.left, -elements[i]->properties.margin.top - elements[i]->el->getSize().y);
			elements[i]->el->setPos(this->pos + Vector2f(0.0f, size.y) + pos);
			elements[i]->el->refresh();
			advancePos.x += elements[i]->properties.margin.left + elements[i]->el->getSize().x + elements[i]->properties.margin.right;
		}
		break;
	case ORDERED_FIT:
		for (int i = 0; i < elements.size(); i++) {
			if (i > 0 && advancePos.x > 0.0f && advancePos.x + elements[i]->properties.margin.left + elements[i]->el->getSize().x + elements[i]->properties.margin.right > size.x) {
				advancePos.x = 0.0f;

				float maxSize = 0.0f;
				int j = i-1;
				while (j >= i-prevLine) {
					maxSize = std::fmax(maxSize, elements[j]->properties.margin.bottom + elements[j]->el->getSize().y + elements[j]->properties.margin.top);
					j--;
				}

				advancePos.y -= maxSize;

				prevLine = 0;
			}
			prevLine++;
			

			Vector2f pos = advancePos + Vector2f(elements[i]->properties.margin.left, -elements[i]->properties.margin.top - elements[i]->el->getSize().y);
			elements[i]->el->setPos(this->pos + Vector2f(0.0f, size.y) + pos);
			elements[i]->el->refresh();
			advancePos.x = pos.x + elements[i]->el->getSize().x + elements[i]->properties.margin.right;
		}

		break;
	case UNORDERED_FIT:
		break;
	}

	allContainedX = true;
	for (int i = 0; i < elements.size() && allContainedX; i++) {
		if (elements[i]->el->getPos().x < pos.x || elements[i]->el->getPos().x + elements[i]->el->getSize().x > pos.x + size.x) {
			allContainedX = false;
		}
	}

	allContainedY = true;
	for (int i = 0; i < elements.size() && allContainedY; i++) {
		if (elements[i]->el->getPos().y < pos.y || elements[i]->el->getPos().y + elements[i]->el->getSize().y > pos.y + size.y) {
			allContainedY = false;
		}
	}

	if (!allContainedX) {
		sliderX.boundPos.x = pos.x;
		sliderX.boundPos.y = pos.y;

		sliderX.boundSize.x = size.x;
		sliderX.boundSize.y = sliderX.size.y;

		/*sliderX.pos.x = pos.x + 0.0f;
		sliderX.pos.y = pos.y + 0.0f;*/

		//sliderX.pos += pos;
	}

	if(!allContainedY){
		sliderY.boundPos.x = pos.x + size.x - sliderY.size.x;
		sliderY.boundPos.y = pos.y;

		sliderY.boundSize.x = sliderY.size.x;
		sliderY.boundSize.y = size.y;

		/*sliderY.pos.x = pos.x + size.x - sliderY.size.x;
		sliderY.pos.y = pos.y + size.y - sliderY.size.y;*/

		//sliderY.pos += pos;
	}

	sliderX.setPos(sliderPerc.x);
	sliderY.setPos(1.0f-sliderPerc.y);

	if (elements.size() > 0) {
		Element* el = elements[elements.size() - 1];
		elemBoundSize = advancePos + Vector2f(0.0f, -el->properties.margin.top - el->el->getSize().y - el->properties.margin.bottom);
	}

	for (int i = 0; i < elements.size(); i++) {
		Element* el = elements[i];
		el->el->setPos(el->el->getPos() - Vector2f(slider.x, slider.y));
		el->el->refresh();
	}
}

void UIContainer::setLayout(Layout layout) {
	if (this->layout != layout) {
		this-> layout = layout;

		refitElements();
	}
}
UIContainer::Layout UIContainer::getLayout() {
	return layout;
}

void UIContainer::resize() {
	sliderPerc = 0.0f;
	slider = 0.0f;
	
	refitElements();

	for (int i = 0; i < elements.size(); i++) {
		elements[i]->el->resize();
		elements[i]->el->refresh();
	}
}

void UIContainer::refresh() {
	sliderX.color = color * 1.3f;
	sliderY.color = color * 1.3f;
}

void UIContainer::setPos(Vector2f pos) {
	if (this->pos != pos) {
		Vector2f oldPos = this->pos;
		this->pos = pos;
		for (int i = 0; i < elements.size(); i++) {
			elements[i]->el->setPos(elements[i]->el->getPos() + (pos - oldPos));
			//elements[i]->el->resize();
			//elements[i]->el->refresh();
		}
	}
}
void UIContainer::setSize(Vector2f size) {
	this->size = size;
	resize();
}

void UIContainer::update() {
	oldMouseInside = mouseInside;

	UIElement::update();

	if (input::getMouseDown(0, inputLayer) || input::getMouseUp(0, inputLayer)) {
		allUnfocused = true;
		for (int i = 0; i < elements.size() && allUnfocused; i++) {
			allUnfocused = !elements[i]->el->isFocused;
		}
	}

	if (input::getMouseInQuadDefault(pos, size) || !allUnfocused || sliderX.locked || sliderY.locked) {
		for (int i = 0; i < elements.size(); i++) {
			elements[i]->el->update();
		}

		if (!allContainedX) {
			sliderX.update();

			if (sliderX.locked) {
				//if (input::getMouseDeltaOrtho().x != 0.0f) {
				if (input::getMouseDelta().x != 0.0f) {
					//sliderX.lockedPos.x += input::getMouseDeltaOrtho().x;
					sliderX.lockedPos.x += input::getMouseDelta().x;

					sliderPerc.x = (sliderX.lockedPos.x - sliderX.boundPos.x) / (sliderX.boundSize.x);
					sliderPerc.x = std::fmin(std::fmax(sliderPerc.x, 0.0f), 1.0f);

					slider.x = elemBoundSize.x*sliderPerc.x;

					refitElements();
				}
			}
		}

		if (!allContainedY) {
			sliderY.update();

			if (sliderY.locked) {
				//if (input::getMouseDeltaOrtho().y != 0.0f) {
				if (input::getMouseDelta().y != 0.0f) {
					//sliderY.lockedPos.y += input::getMouseDeltaOrtho().y;
					sliderY.lockedPos.y += input::getMouseDelta().y;

					sliderPerc.y = (sliderY.lockedPos.y - sliderY.boundPos.y)/(sliderY.boundSize.y);
					sliderPerc.y = std::fmin(std::fmax(sliderPerc.y, 0.0f), 1.0f);
					
					slider.y = elemBoundSize.y*sliderPerc.y;

					refitElements();
				}
			}
		}
	}
}

void UIContainer::draw() {
	glColor4f(color.x, color.y, color.z, color.w);
	util::drawQuad(pos, size);

	util::pushGLScissor(pos, size);

		for (int i = 0; i < elements.size(); i++) {
			elements[i]->el->draw();
		}

		if (!allContainedX) {
			sliderX.draw();
		}

		if (!allContainedY) {
			sliderY.draw();
		}

	util::popGLScissor();
}

void UIContainer::dispose() {
	UIElement::dispose();
	for (int i = 0; i < elements.size(); i++) {
		elements[i]->el->dispose();
		delete elements[i]->el;
		delete elements[i];
	}
	elements.clear();
}
