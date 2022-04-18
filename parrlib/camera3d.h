#pragma once

#include <parrlibcore/vector3f.h>
#include <parrlibcore/quaternion.h>
#include <parrlibcore/matrix4f.h>

#include "input.h"

namespace prb {
	class Camera3D {
	public:
		vec3 pos = 0.f;
		quat rot = pquat::iden;
		mat4 tr = 1.f;

		float speed = 1.f;
		float rotSens = 6.f;

		void input();
		vec3 fw() const;
		vec3 ri() const;
		vec3 up() const;
	};

	typedef Camera3D cam3;
}
