#include "Slider.h"

void Slider::refreshSlider() {
	sliderLinePos = pos + sliderOff + Vector2f(0.0f, (size.y-sliderOff.y*2.0f)/2.0f - size.y*sliderLineWidth/2.0f);
	sliderLineSize = Vector2f((size.x-sliderOff.x*2.0f)*sliderEnd-sliderSpacing, size.y*sliderLineWidth);

	float perc = (val - minVal) / (maxVal - minVal);
	float posFactor = 1.0f;
	if (scaling == Scaling::LINEAR) {
		posFactor = perc;
	}
	else if (scaling == Scaling::EXPONENTIAL) {
		posFactor = (pow(perc, 1.0f/ scalingExpFactor));
	}

	sliderPos = sliderLinePos + Vector2f(sliderLineSize.x*posFactor - size.x*(sliderLength / 2.0f), size.y*sliderLineWidth / 2.0f - (size.y - sliderOff.y*2.0f) / 2.0f);
	sliderPos.x = std::fmin(std::fmax(sliderPos.x, sliderLinePos.x - size.x*(sliderLength / 2.0f)), sliderLinePos.x + sliderLineSize.x - sliderSize.x/2.0f);

	sliderSize = Vector2f(size.x*sliderLength, size.y - sliderOff.y*2.0f);

	txf.setPos(pos + Vector2f(sliderOff.x + sliderLineSize.x + sliderSpacing*2.0f, -size.y*0.03f));
	txf.setSize(Vector2f((size.x)*(1.0f-sliderEnd) - sliderSpacing, size.y+(size.y*0.03f)*2.0f));
	//txf.label.setDynamicScale();

	txf.setVal(val);
}

void Slider::refresh(){
	UIElement::refresh();
	refreshSlider();
}

Slider::Slider()
{
}

Slider::Slider(float curVal, float minVal, float maxVal, TextRendererOld &txr, Vector4f backGroundColor, Vector4f textColor, Vector2f pos, Vector2f size) {
	this->minVal = minVal;
	this->maxVal = maxVal;
	this->color = backGroundColor;

	txf = TextFieldDecimal(val, 0.8f, txr, Vector2f(0.0f), Vector2f(0.0f), Vector4f(0.0f), textColor, [] {});

	txf.setModifier(false);
	txf.setDecimalDigits(2);
	this->pos = pos;
	setSize(size);

	setVal(curVal);
	txf.refreshTextField();
	
	txf.inputTag = inputTag;
}

Slider::~Slider()
{
}

void Slider::setPos(Vector2f pos) {
	if (this->pos != pos) {
		this->pos = pos;
		refreshSlider();
	}
}

Vector2f Slider::getPos() {
	return pos;
}

void Slider::setSize(Vector2f size) {
	this->size = size;
	refreshSlider();
}

Vector2f Slider::getSize() {
	return size;
}

void Slider::setVal(float val) {
	if (this->val != val) {

		if (clampMode == ClampMode::WHOLE) {
			this->val = std::fmin(std::fmax(val, minVal), maxVal);
		}
		else if(clampMode == ClampMode::MIN) {
			this->val = std::fmax(val, minVal);
		}
		else if (clampMode == ClampMode::MAX) {
			this->val = std::fmin(val, maxVal);
		}

		refreshSlider();
	}
}

float Slider::getVal() {
	return val;
}

void Slider::setScalingLinear() {
	if (scaling != Scaling::LINEAR) {
		scaling = Scaling::LINEAR;
		refreshSlider();
	}
}

void Slider::setScalingExponential(float expFactor) {
	if (scaling != Scaling::EXPONENTIAL || (scaling == Scaling::EXPONENTIAL && scalingExpFactor != expFactor)) {
		scaling = Scaling::EXPONENTIAL;
		scalingExpFactor = expFactor;
		refreshSlider();
	}
}

Slider::Scaling Slider::getScaling() {
	return scaling;
}

void Slider::setClampMode(ClampMode clampMode) {
	if (this->clampMode != clampMode) {
		this->clampMode = clampMode;
		refreshSlider();
	}
}

Slider::ClampMode Slider::getClampMode(){
	return clampMode;
}

void Slider::update() {
	UIElement::update();

	

	//setVal(20.0f+sin(gtime::time*2.0f)*10.0f);

	if (input::getMouseDown(0, inputLayer)) {
		if (mouseInSlider) {
			sliderLocked = true;
			//sliderStartLockingMousePos = input::getMouseOrtho().x - (input::getMouseOrtho().x - (sliderPos.x + sliderSize.x / 2.0f));
			sliderStartLockingMousePos = input::getMousePos().x - (input::getMousePos().x - (sliderPos.x + sliderSize.x/2.0f));
			//input::setCursorInputMode(GLFW_CURSOR_DISABLED);
		}
	}

	if (sliderLocked) {
		if (input::getMouseDelta().x != 0.0f) {
			//Vector2f mousePos = input::getMouseOrtho();
			//debug::println(input::getMouseDelta().toString());
			//sliderStartLockingMousePos += input::getMouseDeltaOrtho().x;
			sliderStartLockingMousePos += input::getMouseDelta().x;

			float perc = (sliderStartLockingMousePos - sliderLinePos.x) / (sliderLineSize.x);
			perc = std::fmin(std::fmax(perc, 0.0f), 1.0f);

			if (scaling == Scaling::LINEAR) {
				setVal(minVal + perc * (maxVal - minVal));
			}
			else if(scaling == Scaling::EXPONENTIAL) {
				setVal(minVal + (pow(perc, scalingExpFactor)) * (maxVal - minVal));
			}
		}
	}

	if (input::getMouseUp(0, inputLayer)) {
		sliderLocked = false;
		//input::setCursorInputMode(GLFW_CURSOR_NORMAL);
	}

	if (txf.isActive()) {
		if (input::getKeyDown(GLFW_KEY_ENTER, txf.inputLayer) || (input::getMouseDown(0, inputLayer) && !input::getMouseInQuad(pos, size))) {
			setVal(txf.getVal());
		}
	}

	txf.update();

	mouseInSlider = input::getMouseInQuad(sliderPos, sliderSize);
}

void Slider::draw() {
	Vector4f tCol = color;

	drawBackground(this);

	Vector4f sliderLineCol = tCol / 4.0f;
	glColor4f(sliderLineCol.x, sliderLineCol.y, sliderLineCol.z, color.w);
	util::drawQuad(sliderLinePos, sliderLineSize);

	Vector4f sliderCol = tCol * 1.2f;
	if (sliderLocked) {
		sliderCol /= 2.1f;
	}
	else {
		if (mouseInSlider) {
			sliderCol /= 1.7f;
		}
	}

	glColor4f(sliderCol.x, sliderCol.y, sliderCol.z, color.w);
	util::drawQuad(sliderPos, sliderSize);

	txf.draw();
}
