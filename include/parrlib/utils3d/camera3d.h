#pragma once

#include <parrlib/vector3f.h>
#include <parrlib/quaternion.h>
#include <parrlib/matrix4f.h>

class Camera3D {
public:
	vec3 pos = 0.f;
	quat rot = 1.f;
	mat4 tr = 1.f;

	float speed = 1.f;

	void input();
	vec3 fw() const;
	vec3 ri() const;
	vec3 up() const;
};

typedef Camera3D cam3;
