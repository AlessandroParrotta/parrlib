#include "indexbuffer.h"

namespace prb {
	IndexBuffer::IndexBuffer(){}
	IndexBuffer::IndexBuffer(std::vector<unsigned int> const& idxs) { ptr = impl::createIndexBuffer_1(idxs); }

	void IndexBuffer::bind() { ptr->bind(); }
	void IndexBuffer::unbind() { ptr->unbind(); }

	void IndexBuffer::setData(std::vector<unsigned int> const& data) { ptr->setData(data); }
	void IndexBuffer::resize(size_t size) { ptr->resize(size); }

	void IndexBuffer::draw() { ptr->draw(); }

	void IndexBuffer::dispose() { ptr->dispose(); }
}
