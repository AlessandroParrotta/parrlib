#include "indexbuffer.h"

namespace dx11 {

	IndexBuffer::IndexBuffer(std::vector<unsigned int> const& idxs) {
		this->data = idxs;

		// Fill in a buffer description.
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(unsigned int) * idxs.size();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &idxs[0];
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the buffer with the device.
		ThrowIfFailed(dev->CreateBuffer(&bufferDesc, &InitData, &buf));
	}

	void IndexBuffer::bind() {
		devcon->IASetIndexBuffer(buf, DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBuffer::unbind(){
	
	}

	void IndexBuffer::setData(std::vector<unsigned int> const& data) {
		this->data = data;

		D3D11_MAPPED_SUBRESOURCE ms;
		ThrowIfFailed(devcon->Map(buf, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

			memcpy(ms.pData, &data[0], sizeof(unsigned int) * data.size());

		devcon->Unmap(buf, NULL);
	}



	void IndexBuffer::resize(size_t size) {
		if (data.size() < size) setData(std::vector<unsigned int>(data.begin(), data.begin()+size));
		else if(data.size() > size) {
			std::vector<unsigned int> nData(data.begin(), data.end());
			nData.reserve(data.size() + (size - data.size()));
			for (int i = 0; i < size - data.size(); i++) nData.push_back(0);
			setData(nData);
		}
	}

	void IndexBuffer::draw() {
		bind();
		devcon->DrawIndexed(data.size(), 0, 0);
	}

	void IndexBuffer::dispose() {
		buf->Release();
	}

	extern "C" __declspec(dllexport) impl::IndexBuffer_base * createIndexBuffer_1(std::vector<unsigned int> const& idxs) { return new IndexBuffer(idxs); }
}
