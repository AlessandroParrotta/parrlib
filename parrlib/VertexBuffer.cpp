#include "VertexBuffer.h"

#include "debug.h"

namespace prb {
	VertexBuffer::VertexBuffer() { /*ptr = impl::createVertexBuffer_1();*/ }
	VertexBuffer::VertexBuffer(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, unsigned int offset) { this->dataTypes = dataTypes; ptr = impl::createVertexBuffer_2(data, byteSize, dataTypes, offset); }
	VertexBuffer::VertexBuffer(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, unsigned int  offset) { this->dataTypes = dataTypes;  ptr = impl::createVertexBuffer_3(data, dataTypes, offset); }
	VertexBuffer::VertexBuffer(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes) : VertexBuffer(data, dataTypes, 0) {}
	VertexBuffer::VertexBuffer(size_t byteSize, std::vector<unsigned int> const& dataTypes, unsigned int offset) { this->dataTypes = dataTypes;  ptr = impl::createVertexBuffer_4(byteSize, dataTypes, offset); }

	VertexBuffer::VertexBuffer(std::vector<unsigned int> const& dataTypes) {
		this->dataTypes = dataTypes;
		size_t byteSize = 0;
		for (int i = 0; i < dataTypes.size(); i++) byteSize += sizeof(float) * dataTypes[i];
		if (byteSize == 0) byteSize = sizeof(float);

		//deb::debss << "constructing vbuf " << dataTypes.size() << "\n"; deb::outStr();

		ptr = impl::createVertexBuffer_4(byteSize, dataTypes, 0);
	}

	void VertexBuffer::resize(size_t size) { ptr->resize(size); }
	void VertexBuffer::setData(std::vector<float> const& data) { ptr->setData(data, dataTypes); }
	void VertexBuffer::setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes) { this->dataTypes = dataTypes; ptr->setData(data, dataTypes); }

	void VertexBuffer::use() { ptr->use(); }
	void VertexBuffer::release() { ptr->release(); }

	void VertexBuffer::draw() { ptr->draw(); }

	void VertexBuffer::dispose() { ptr->dispose(); delete ptr; }
}
