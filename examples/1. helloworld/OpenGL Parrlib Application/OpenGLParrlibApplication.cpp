// OpenGLParrlibApplication.cpp : Defines the entry point for the console application.
//

//keep in mind: you can refer to the main context via 'Context::' or 'prc::'

#include <iostream>
#include <math.h>

#include <parrlib/parrlib.h>

TextRenderer txr;

void init() {
	prc::setClearColor("#ffffffff");
}

void update() {
	util::setColor("#7C7C7C");

	float rotationSpeed = 10.f;
	mat3 quadRotation = pmat3::rotate(gtime::time * rotationSpeed);

	vec2 size = .5f;
	if (input::getMouse(GLFW_MOUSE_BUTTON_1)) {	//if the left mouse button is pressed
		//set the size to the distance of the mouse from the center of the screen (rotated by the inverse of the rotation of the quad)
		size = quadRotation.inverted() * input::getMousePos();
	}

	util::drawQuad(aabb2(-size, size), quadRotation);

	util::text << "Hello!";
	util::drawText(36, vec2(0.f));

	std::vector<int> keys = input::getPressedKeys();
	if (keys.size() > 0) {
		for (auto& k : keys) util::text << "key " << (std::wstring() + (wchar_t)k) << " pressed!\n";
		util::drawText(24, input::getMousePos());
	}

	//unsigned int k = input::pollTextKey();
	//if (k > 0) {
	//	util::text << "key " << (std::wstring() + (wchar_t)k) << " pressed!\n";
	//	util::drawText(24, input::getMousePos());
	//}
}

int main(){
	prc::setVSync(true);
	prc::setup({ &init, &update });
}
