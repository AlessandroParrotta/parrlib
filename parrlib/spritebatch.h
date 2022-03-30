#pragma once

#include "texture.h"
//#include "vertexarray.h"

#include "sprite.h"

namespace prb {
	class SpriteBatch {
	public:
		Sprite atlas;

		mat3 transform = 1.f;

		std::vector<float> data;
		//VAO vao;

		Shader sh;

		int curFloat = 0;

		void init();
		SpriteBatch();
		SpriteBatch(Sprite const& atlas);

		void draw(Sprite const& s, vec2 const& txmin, vec2 const& txmax, mat3 const& transform);
		void draw(Sprite const& s, Sprite::AnimationPlayer const& sa, mat3 const& transform);
		void draw(Sprite const& s, mat3 const& transform);
		void flush();
		void clear();
		void flushAndClear();
	};

}
