#include <parrlib/utils2d/GameObject.h>


#include <parrlib/utils2d/Poly.h>

GameObject::GameObject(){

}

GameObject::GameObject(Vector2f pos) {
	this->pos = pos;
}

GameObject::GameObject(Vector2f pos, float rot){
	this->pos = pos;
	this->rot = rot;
}

GameObject::~GameObject()
{
}

void GameObject::setPos(Vector2f pos) {
	oldPos = this->pos;
	this->pos = pos;
}

Vector2f GameObject::getPos() {
	return pos;
}


void GameObject::setRot(float rot) {
	oldRot = this->rot;
	this->rot = rot;
}

float GameObject::getRot() {
	return rot;
}

bool GameObject::isSleeping() {
	return sleeping;
}

void GameObject::putToSleep() {
	sleeping = true;
}

void GameObject::wakeUp() {
	sleeping = false;
}

void GameObject::update() {
	if (!isSleeping()) {
		if (oldPos == pos && oldRot == rot) {
			sleepTimer += gtime::deltaTime;
			if (sleepTimer > sleepTime) {
				putToSleep();
				sleepTimer = 0.0f;
			}
		}
	}
	else {
		if (oldPos != pos || oldRot != rot) {
			wakeUp();
			sleepTimer = 0.0f;
		}
	}

	//Debug::drawCircle(Consts::project(pos), 0.1f*Consts::zoom, isSleeping() ? Vector4f(1.0f,0.0f,0.0f,1.0f) : Vector4f(0.0f,1.0f,0.0f,1.0f));
}

void GameObject::draw(Vector2f camPos, float zoom) {

}

std::vector<std::shared_ptr<Poly>> GameObject::getPoly() {
	return std::vector<std::shared_ptr<Poly>>(0);
}