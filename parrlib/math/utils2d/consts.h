#pragma once

#include "..\Vector2f.h"
#include "..\Timer.h"

#include "AxisAlignedBoundingBox2D.h"

namespace prb {
	//#define COLLIDER_DEBUG
	//#define VDEBUGEN
	//#define BDEBUGEN
	//#define VLDEBUGEN
	//#define BLDEBUGEN

	//#define OPTIMIZE_CONVEX
	//#define ENABLE_DECOLLISION

	namespace consts {
		extern Vector2f targetCamPos;
		extern Vector2f camPos;
		extern float camPosSpeed;
		extern aabb2 camBoundingBox;

		extern int maxTrashPolys;
		extern bool flatShading;
		extern bool triangulateMeshes;

		extern float targetZoom;
		extern float zoom;
		extern float zoomSpeed;

		extern float polyPerThreshold;
		extern float polyAreaThreshold;
		extern float polyDelThreshold;

		extern float collisionOffset;

		Vector2f project(Vector2f point);
	};
}