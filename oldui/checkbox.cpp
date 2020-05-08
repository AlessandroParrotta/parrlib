#include "CheckBox.h"

void CheckBox::update(){
	if (input::getMouseUp(0, inputLayer)) {
		if (locked) {
			checked = !checked;
		}
	}

	UIElement::update();

	if (input::getMouseDown(0, inputLayer)) {
		if (mouseInside) {
			locked = true;
		}
	}

	if (input::getMouseUp(0, inputLayer)) {
		if (locked) {
			if (mouseInside) {
				action();
			}

			locked = false;
		}
	}
}

void CheckBox::draw(){
	drawBackground(this);
	
	if (checked) {
		Vector4f tCol = color.colDiv(locked ? (mouseInside ? 4.f : 3.f) : (mouseInside ? 2.5f : 1.5f));

		util::setColor4f(tCol);
		//util::drawQuad(pos+size*offSize, size-size*offSize*2.0f);
		vec2 tpos = pos + size * offSize;
		vec2 tsize = size - size * offSize*2.0f;
		util::drawQuad(tpos, tsize);
		//vec2 joinPos = tpos + vec2(tsize.x*.5f, tsize.y*.35f);
		//util::drawQuad(tpos+vec2(tsize.x*.2f,tsize.y*.6f), vec2(tsize.x*.3f,tsize.y*.3f), joinPos);
		//util::drawQuad(tpos + vec2(tsize.x*.6f, tsize.y*.65f), vec2(tsize.x*.65f, tsize.y*.3f), joinPos);
	}
}

CheckBox::CheckBox() {

}

CheckBox::CheckBox(Vector2f pos, Vector2f size, Vector4f color) : UIElement(){
	setPos(pos);
	setSize(size);
	setColor(color);
}

CheckBox::CheckBox(std::function<void()> action) : action(action) {}


CheckBox::~CheckBox(){

}
