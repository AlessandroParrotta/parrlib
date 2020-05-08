#include "UIManager.h"



UIManager::UIManager()
{
}

UIManager::UIManager(std::vector<UIElement*> &uiElems){
	this->uiElems = uiElems;
}

UIManager::~UIManager()
{
}

void UIManager::addElem(int at, UIElement* uiElem) {
	uiElems.insert(uiElems.begin() + at, uiElem);
}

void UIManager::addElem(UIElement* uiElem) {
	uiElems.push_back(uiElem);
}

void UIManager::removeElem(int i) {
	uiElems[i]->dispose();
	delete uiElems[i];
	uiElems.erase(uiElems.begin() + i);
}

void UIManager::removeElems(int from, int to) {
	for (int i = from; i < to; i++) {
		uiElems[i]->dispose();
		delete uiElems[i];
	}
	uiElems.erase(uiElems.begin() + from, uiElems.begin()+to);
}

UIElement* UIManager::at(int i) {
	return uiElems[i];
}

int UIManager::getElemCount() {
	return uiElems.size();
}

void UIManager::update() {
	for (int i = 0; i < uiElems.size(); i++) {
		uiElems[i]->update();
	}
}

void UIManager::draw(){
	for (int i = 0; i < uiElems.size(); i++) {
		uiElems[i]->draw();
	}
}

void UIManager::dispose() {
	for (int i = 0; i < uiElems.size(); i++) {
		uiElems[i]->dispose();
		delete uiElems[i];
	}
}
