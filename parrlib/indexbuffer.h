#pragma once

#include "impl/impl.h"

namespace prb {
	class IndexBuffer {
	public:
		impl::IndexBuffer_base* ptr = NULL;

		IndexBuffer();
		IndexBuffer(std::vector<unsigned int> const& idxs);

		void bind();
		void unbind();

		void setData(std::vector<unsigned int> const& data);
		void resize(size_t size);

		void draw();

		void dispose();
	};

	typedef IndexBuffer ibuf;
}
