#pragma once

#include <vector>

#include <parrlibcore/vector2f.h>

namespace prb {
	namespace impl {

		class Texture_base {
		public:
			//virtual ~Texture_base();
			virtual void fillRegion(unsigned char* subData, vec2 start, vec2 end) = 0;
			virtual std::vector<unsigned char> getData() const = 0;

			virtual void resize(vec2 newSize) = 0;
			virtual vec2 getSize() const = 0;

			virtual void setData(unsigned char* data, vec2 size, int linesize) = 0;
			virtual void setData(unsigned char* data, vec2 size) = 0;
			virtual void drawImmediate(std::vector<float> const& verts) const = 0;
			virtual void drawImmediate(vec2 pos, vec2 size) const = 0;
			virtual void bind() const = 0;
			virtual void dispose() = 0;

			virtual bool null() const = 0;
		};

	}
}
