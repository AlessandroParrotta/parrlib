#include <parrlib/utils2d/Consts.h>

namespace consts {
	Vector2f targetCamPos = Vector2f();
	Vector2f camPos = targetCamPos;
	float camPosSpeed = 6.7f;
	aabb2 camBoundingBox = aabb2(0.0f,0.01f);

	int maxTrashPolys = 1000;
	bool flatShading = false;
	bool triangulateMeshes = true;

	float targetZoom = 1.0f;
	float zoom = targetZoom;
	float zoomSpeed=15.0f;

	float polyPerThreshold=0.05f; //perimeter threshold
	float polyAreaThreshold = 0.0015f;
	float polyDelThreshold=polyAreaThreshold*0.01f;

	float collisionOffset = 0.0001f;

	Vector2f project(Vector2f point){
		return (point - camPos) * zoom;
	}
};