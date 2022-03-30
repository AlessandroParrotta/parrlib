#pragma once

#include <vector>

namespace prb {
	namespace impl {
		class VertexBuffer_base {
		public:
			//virtual ~VertexBuffer_base();
			virtual void resize(size_t size) = 0; //in bytes
			virtual void setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes) = 0;

			virtual void use() = 0;
			virtual void release() = 0;

			virtual void draw() = 0;

			virtual void dispose() = 0;
		};
	}
}
