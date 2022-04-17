#include "camera3d.h"

#include "debug.h"
#include "time.h"

namespace prb {
	void Camera3D::input() {
		int sw = input::getMWheel();
		if (sw > 0) speed *= 2.f;
		if (sw < 0) speed /= 2.f;

		vec3 iaxis = input::getIAxis3();
		pos += (fw() * iaxis.z + ri() * iaxis.x + up() * iaxis.y) * speed * gtime::deltaTime;

		if (input::getKeyDown('Z')) input::toggleMouseStatus();
		if (input::getMouseStatus() == input::MOUSE_LOCKED) {
			vec3 sens = vec3(input::getMouseDeltaConst(), .5f * gtime::deltaTime) * rotSens;
			//rot = pquat::lookAt(pos, 0.f, quat(gtime::time, fw()) * v3ax::upv3);
			rot *=	quat(-sens.x, v3ax::upv3) *
					quat(-sens.y, v3ax::riv3) *
					quat((input::getKey('Q') ? sens.z : input::getKey('E') ? -sens.z : 0.f), v3ax::fwv3);
		}

		tr = rot * pmat4::translate(-pos);
	}

	vec3 Camera3D::fw() const { return tr.transposed() * -v3ax::fwv3; }
	vec3 Camera3D::ri() const { return tr.transposed() * v3ax::riv3; }
	vec3 Camera3D::up() const { return tr.transposed() * v3ax::upv3; }
}
