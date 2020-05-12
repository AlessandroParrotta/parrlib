#include <parrlib/utils3d/camera3d.h>

#include <parrlib/debug.h>

void Camera3D::input() {
	int sw = input::getMouseScrollWheel();
	if (sw > 0) speed *= 2.f;
	if (sw < 0) speed /= 2.f;

	vec3 iaxis = input::getIAxis3();
	pos += (fw() * iaxis.z + ri() * iaxis.x + up() * iaxis.y) * speed * gtime::deltaTime;

	if (input::getKeyDown(GLFW_KEY_Z)) input::toggleCursorInputMode();
	if (input::getCursorInputMode() == GLFW_CURSOR_DISABLED) {
		vec3 sens = vec3(input::getMouseDeltaConst(), .5f) * 6.f * gtime::deltaTime;
		//rot = pquat::lookAt(pos, 0.f, quat(gtime::time, fw()) * v3ax::upv3);
		rot *= quat(-sens.x, v3ax::upv3) *
			quat(-sens.y, v3ax::riv3) *
			quat((input::getKey(GLFW_KEY_Q) ? sens.z : input::getKey(GLFW_KEY_E) ? -sens.z : 0.f), v3ax::fwv3);
	}

	tr = rot * pmat4::translate(-pos);
}

vec3 Camera3D::fw() const { return tr.transposed() * -v3ax::fwv3; }
vec3 Camera3D::ri() const { return tr.transposed() * v3ax::riv3; }
vec3 Camera3D::up() const { return tr.transposed() * v3ax::upv3; }
