#pragma once

#include <fstream>
#include <unordered_map>
#include <array>

#include "texture.h"
#include "math/matrix3f.h"
#include "time.h"

#include "math/stringutils.h"
#include "math/otherutil.h"

#include "debug.h"

#include "util.h"

namespace prb {
	class Sprite {
	public:
		Texture tex;
		vec2 size = 0.f;			//size of the sprite in integer pixels (might differ from tex.size)

		class Animation {
		public:
			vec2 coord = 0.f;	//starting coordinate, in sprite coordinates
			int frames = 1;		//number of frames
			int fps = 0;		//how many fps this animation runs at
			bool loop = false;

			bool tiled = false;		//wheter this is a tiled sprite
			aabb2 tiledarea = 0.f;	//bounding box of the area to tile

			bool operator== (Animation const& o) const { return coord == o.coord && frames == o.frames && fps == o.fps && loop == o.loop; }
		};
		std::unordered_map<std::string, Animation> anims;

		class AnimationPlayer {
			Animation anim;		//pointer to the animation to play
		public:
			int curFrame = 0;				//current frame in the animation
			float curTime = 0.f;			//current time in the animation (in seconds)
			float speed = 1.f;
			bool finished = false;
			bool loop = false;

			AnimationPlayer() {}
			AnimationPlayer(Animation const& anim) : anim(anim) { loop = anim.loop; }
			AnimationPlayer(Sprite const& s, std::string const& animName) { anim = s.anims.at(animName); loop = anim.loop; }

			void reset() {
				curFrame = 0;
				curTime = 0.f;
				finished = false;
			}

			void advance() {
				//if (!anim) return;

				if (!loop && curFrame >= anim.frames - 1) { finished = true; return; }

				curTime += gtime::deltaTime * speed;

				float animDuration = (1.f / anim.fps) * anim.frames; //in seconds
				curFrame = (int)(anim.frames * (curTime / animDuration));

				if (loop && curFrame >= anim.frames) { reset(); }
			}

			void setAnim(Animation const& anim) {
				if (this->anim == anim) return;

				loop = anim.loop;

				this->anim = anim;
				reset();
			}
			void setAnim(Sprite s, std::string const& animName) { setAnim(s.anims[animName]); }

			Animation getAnim() const { return anim; }
			aabb2 getTiledArea() const { return anim.tiledarea; }
		};

		Sprite();
		Sprite(Texture const& tex);
		Sprite(const char* tex);
		Sprite(Texture const& tex, vec2 const& size);

		aabb2 getTiledAreaScreenSpace(Sprite::AnimationPlayer const& ap, vec2 const& size);
		aabb2 getTiledAreaScreenSpace(std::string const& state, vec2 const& size);

		vec2 getOffset(AnimationPlayer const& sa) const;
		std::array<aabb2, 9> getTiledOffsets(AnimationPlayer const& sa) const;

		void draw(std::vector<float> const& batch) const;
		void draw(mat3 const& transform) const;
		//void draw(aabb2 const& tx, mat3 const& transform) const;
		//void draw(vec2 const& txmin, vec2 const& txmax, mat3 const& transform) const;
		void draw(AnimationPlayer const& ap, mat3 const& transform) const;
		void draw(std::string const& statename, mat3 const& transform) const;

		void draw(aabb2 const& bb, mat3 const& transform) const;
		void draw(AnimationPlayer const& ap, aabb2 const& bb, mat3 const& transform) const;
		void draw(std::string const& statename, aabb2 const& bb, mat3 const& transform) const;

		void drawTiled(AnimationPlayer const& ap, vec2 const& size, mat3 const& transform) const;
		void drawTiled(std::string const& state, vec2 const& size, mat3 const& transform) const;

		void drawTiled(AnimationPlayer const& ap, aabb2 const& bb, mat3 const& transform) const;
		void drawTiled(std::string const& state, aabb2 const& bb, mat3 const& transform) const;
	};
}
