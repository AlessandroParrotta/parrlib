#include "OrthoCoord.h"



OrthoCoord::OrthoCoord()
{
}

OrthoCoord::OrthoCoord(float left, float right, float bottom, float top, float zNear, float zFar) {
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->zNear = zNear;
	this->zFar = zFar;
}

OrthoCoord::~OrthoCoord()
{
}

void OrthoCoord::set() {
	oldLeft = left, oldRight = right;
	oldBottom = bottom, oldTop = top;
	oldZNear = zNear, oldZFar = zFar;

	glOrtho(left, right, bottom, top, zNear, zFar);
}

void OrthoCoord::reset() {
	//left = oldLeft, right = oldRight;
	//bottom = oldBottom, top = oldTop;
	//zNear = oldZNear, zFar = oldZFar;

	glOrtho(oldLeft, oldRight, oldBottom, oldTop, oldZNear, oldZFar);
}

void OrthoCoord::calcScale(float refX, float refY) {
	scaleX = refX/(right - left);
	scaleY = refY/(top - bottom);
}