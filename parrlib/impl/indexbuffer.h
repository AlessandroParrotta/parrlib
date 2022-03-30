#pragma once

namespace prb {
	namespace impl {
		class IndexBuffer_base {
		public:
			virtual void bind() = 0;
			virtual void unbind() = 0;

			virtual void setData(std::vector<unsigned int> const& data) = 0;
			virtual void resize(size_t size) = 0;

			virtual void draw() = 0;

			virtual void dispose() = 0;
		};
	}
}
