#include "vertexbuffer.h"

#include "common.h"
#include "../parrlib/debug.h"

namespace dx11 {
	void VertexBuffer::init(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subRes) {
		ThrowIfFailed(dev->CreateBuffer(desc, subRes, &vptr));
	}

	VertexBuffer::VertexBuffer() {}
	VertexBuffer::VertexBuffer(D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA& subRes) { init(&desc, &subRes); }
	VertexBuffer::VertexBuffer(void* data, size_t byteSize, UINT stride, UINT offset) {
		this->stride = stride;
		this->offset = offset;

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = byteSize;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subRes;
		subRes.pSysMem = data;
		subRes.SysMemPitch = 0;
		subRes.SysMemSlicePitch = 0;

		dataCapacity = byteSize;
		vCount = dataCapacity / stride;

		init(&desc, &subRes);
	}

	VertexBuffer::VertexBuffer(std::vector<float> const& data, UINT stride, UINT offset) {
		this->stride = stride;
		this->offset = offset;

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(float) * data.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subRes;
		subRes.pSysMem = &data[0];
		subRes.SysMemPitch = 0;
		subRes.SysMemSlicePitch = 0;

		dataCapacity = data.size() * sizeof(float);
		vCount = dataCapacity / stride;

		//ThrowIfFailed(dev->CreateBuffer(&desc, NULL, &vptr));
		init(&desc, &subRes);

		//deb::debss << "init vb data " << data.size() << " (" << (data.size()*sizeof(float)) << ") " << stride << " " << offset; deb::msbinfo();
	}

	VertexBuffer::VertexBuffer(size_t byteSize, UINT stride, UINT offset) {
		this->stride = stride;
		this->offset = offset;

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = byteSize;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		dataCapacity = byteSize;
		vCount = dataCapacity / stride;

		init(&desc, NULL);

		//deb::debss << "init vb " << byteSize << " " << stride << " " << offset; deb::msbinfo();
 		//deb::msbinfo("init vertex buffer");
	}

	void VertexBuffer::resize(size_t size) {
		//deb::ss << "Resize!! " << size << " " << curtime << "\n";
		vbuf vb(size, stride, offset);
		dispose();
		*this = vb;
	}

	void VertexBuffer::setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes) {
		if (data.size() <= 0) return;

		if (data.size() * sizeof(float) > dataCapacity) resize(data.size() * sizeof(float));
		else if (data.size() * sizeof(float) < dataCapacity / 4) resize(data.size() * sizeof(float));
		else vCount = data.size() / (stride / sizeof(float));

		D3D11_MAPPED_SUBRESOURCE ms;
		ThrowIfFailed(devcon->Map(vptr, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

			memcpy(ms.pData, &data[0], sizeof(float) * data.size());

			//deb::debss << "vb set data memcpy " << ms.pData << " " << (&data[0]) << " " <<(sizeof(float) * data.size()); deb::msbinfo();

		devcon->Unmap(vptr, NULL);

		//deb::debss << "vb set data " << data.size(); deb::msbinfo();
	}

	void VertexBuffer::use() {
		devcon->IASetVertexBuffers(0, 1, &vptr, &stride, &offset);
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//deb::debss << "vb use "; deb::msbinfo();
	}

	void VertexBuffer::release() {
		//devcon->IASetVertexBuffers(0, 1, 0, &stride, &offset);
	}

	void VertexBuffer::draw() {
		use();
		devcon->Draw(vCount, 0);
		//deb::debss << "vertexbuffer draw " << vCount << " " << vptr << " " << dataCapacity << " " << stride << " " << offset; deb::msbinfo();
	}

	void VertexBuffer::dispose() {
		vptr->Release();
	}

	size_t calcStride(std::vector<unsigned int> const& dataTypes) {
		size_t stride = 0; for (int i = 0; i < dataTypes.size(); i++) stride += dataTypes[i];
		stride *= sizeof(float);
		return stride;
	}

	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_1() { return new VertexBuffer(); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_2(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) {  return new VertexBuffer(data, byteSize, calcStride(dataTypes), offset); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_3(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset) { return new VertexBuffer(data, calcStride(dataTypes), offset); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_4(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) { return new VertexBuffer(byteSize, calcStride(dataTypes), offset); }

}
