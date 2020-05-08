#pragma once

#include "Vector2f.h"
#include "utils2d/axisalignedboundingbox2d.h"

namespace constants {
	extern float textSizeThreshold;

	void res(vec2 r);

	float resx();
	float resy();
	//returns the resolution of the currently bound framebuffer
	//note that if you do not use FrameBuffer.bind() (and unbind()) to bind your framebuffers, this value will most likely be wrong
	vec2 res();

	//returns the resolution of the currently bound framebuffer as a boundingbox ({ {0,0} -> {resX,resY} })
	aabb2 resbb();

	float resaspectx();
	float resaspecty();
	float resaspect();

}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace cst = constants;
#endif
