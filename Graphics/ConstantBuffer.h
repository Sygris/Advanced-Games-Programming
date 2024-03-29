#pragma once

#include <d3d11.h>
#include <wrl/client.h> // ComPtr
#include "ConstantBufferTypes.h"

template<class T>
class ConstantBuffer
{
public:
	ConstantBuffer() {}

	T data;

	ID3D11Buffer* Get() const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return m_buffer.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		// Prevents memory leak
		if (m_buffer.Get() != nullptr)
			m_buffer.Reset();

		m_deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, m_buffer.GetAddressOf());
		return hr;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = m_deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		
		if (FAILED(hr))
		{
			OutputDebugString("Failed to map constant buffer.");
			return false;
		}

		CopyMemory(mappedResource.pData, &data, sizeof(T));
		m_deviceContext->Unmap(m_buffer.Get(), 0);
		return true;
	}
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	ID3D11DeviceContext* m_deviceContext = nullptr;
};