#pragma once

namespace prb {
	class OrthoCoord {
	public:
		float left = 0.0f, right = 0.0f;
		float bottom = 0.0f, top = 0.0f;
		float zNear = 0.0f, zFar = 0.0f;
		float scaleX = 0.0f, scaleY = 0.0f;

		float oldLeft = left, oldRight = right;
		float oldBottom = bottom, oldTop = top;
		float oldZNear = zNear, oldZFar = zFar;
		float oldScaleX, oldScaleY;

		OrthoCoord();
		OrthoCoord(float left, float right, float bottom, float top, float zNear, float zFar);

		~OrthoCoord();

		void set();
		void reset();

		void calcScale(float refX, float refY);
	};

}
