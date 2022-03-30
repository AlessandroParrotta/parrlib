#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>


#include "common.h"
//#include "../parrlib/debug.h"

namespace dx11 {
	class ConstantBuffer {
	public:
		ID3D11Buffer* cptr = NULL;

		void init(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA subRes);

		ConstantBuffer();
		ConstantBuffer(D3D11_BUFFER_DESC& desc, D3D11_SUBRESOURCE_DATA& subRes);
		ConstantBuffer(const void* data, UINT byteSize);
		template<class T> ConstantBuffer(T const& data) : ConstantBuffer(&data, sizeof(data)) {}

		void setData(const void* data, UINT offset, UINT byteSize);
		void setData(const void* data, UINT byteSize);

		template<class T> void setData(T const& data) {
			D3D11_MAPPED_SUBRESOURCE ms;
			ThrowIfFailed(devcon->Map(cptr, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

			memcpy(ms.pData, &data, sizeof(data));

			devcon->Unmap(cptr, NULL);
		}

		void use() const;

		void dispose();
	};

	typedef ConstantBuffer cbuf;

}
