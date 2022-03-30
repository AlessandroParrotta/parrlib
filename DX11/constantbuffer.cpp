#include "constantbuffer.h"

#include "../parrlib/debug.h"

namespace dx11 {
	using namespace prb;

	void ConstantBuffer::init(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA subRes) {
		ThrowIfFailed(dev->CreateBuffer(&desc, &subRes, &cptr));
	}

	ConstantBuffer::ConstantBuffer() {}
	ConstantBuffer::ConstantBuffer(D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA& subRes) { init(desc, subRes); }
	ConstantBuffer::ConstantBuffer(const void* data, UINT byteSize) {
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = byteSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subRes;
		subRes.pSysMem = data;
		subRes.SysMemPitch = 0;
		subRes.SysMemSlicePitch = 0;

		ThrowIfFailed(dev->CreateBuffer(&desc, &subRes, &cptr));
	}

	void ConstantBuffer::setData(const void* data, UINT offset, UINT byteSize) {
		//for (int i = 0; i < byteSize; i++) deb::debss << (int)((byte*)data)[i] << " ";
		//deb::debss << "\n"; deb::outStr();

		D3D11_MAPPED_SUBRESOURCE ms;
		ThrowIfFailed(devcon->Map(cptr, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

			memcpy((void*)(((BYTE*)ms.pData) + offset), data, byteSize);

		devcon->Unmap(cptr, NULL);
	}

	void ConstantBuffer::setData(const void* data, UINT byteSize) {
		D3D11_MAPPED_SUBRESOURCE ms;
		ThrowIfFailed(devcon->Map(cptr, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

			memcpy(ms.pData, data, byteSize);

		devcon->Unmap(cptr, NULL);
		//devcon->UpdateSubresource(cptr, 0, 0, data, 0, 0);
	}

	void ConstantBuffer::use() const {
		devcon->VSSetConstantBuffers(0, 1, &cptr);
		devcon->PSSetConstantBuffers(0, 1, &cptr);
	}

	void ConstantBuffer::dispose() {
		if (!cptr) return;

		cptr->Release();
	}

}
