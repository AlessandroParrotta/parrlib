#pragma once

#include <vector>

#include <parrlibcore/Vector2f.h>
#include <parrlibcore/physics2d/collider2d.h>

namespace prb {
	class Poly;

	class GameObject {
	protected:
		Vector2f pos;
		Vector2f oldPos;

		float rot;
		float oldRot;

		bool sleeping = false;

		float sleepTimer = 0.0f;
		float sleepTime = 2.0f;

	public:
		std::shared_ptr<Collider2D> col = nullptr;

		GameObject();
		GameObject(Vector2f pos);
		GameObject(Vector2f pos, float rot);

		~GameObject();

		virtual void setPos(Vector2f pos);
		virtual Vector2f getPos();

		virtual void setRot(float rot);
		virtual float getRot();

		virtual bool isSleeping();

		virtual void putToSleep();
		virtual void wakeUp();

		virtual void update();
		virtual void draw(Vector2f camPos, float zoom);

		virtual std::vector<std::shared_ptr<Poly>> getPoly();
	};

	typedef GameObject gobj;
}
