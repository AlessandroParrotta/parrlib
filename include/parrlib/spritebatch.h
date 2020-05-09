#pragma once

#include <Parrlib/Texture.h>
#include <Parrlib/VertexArrayObject.h>

#include "Sprite.h"

class SpriteBatch{
public:
	Sprite atlas;

	mat3 transform = 1.f;

	std::vector<float> data;
	VAO vao;

	Shader sh;

	int curFloat = 0;

	void init();
	SpriteBatch();
	SpriteBatch(Sprite const& atlas);

	void SpriteBatch::draw(Sprite const& s, vec2 const& txmin, vec2 const& txmax, mat3 const& transform);
	void draw(Sprite const& s, Sprite::AnimationPlayer const& sa, mat3 const& transform);
	void draw(Sprite const& s, mat3 const& transform);
	void flush();
	void clear();
	void flushAndClear();
};

