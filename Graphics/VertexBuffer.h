#pragma once
#include <d3d11.h>
#include <wrl/client.h> // ComPtr
#include <memory>

// FUTURE: Make this class work with Index Buffers/ConstantBufffer too (Pass USAGE and BINDS in the parameters)
// What about the Stride? Vertex use it index don't, best way to do this?

template<class T>
class VertexBuffer
{
public:
	VertexBuffer() {}

	ID3D11Buffer* Get() const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return m_buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return m_bufferSize;
	}

	const UINT Stride() const
	{
		return *m_stride.get();
	}

	const UINT* StridePtr() const
	{
		return m_stride.get();
	}

	HRESULT Initialise(ID3D11Device* device, T* data, UINT numOfVertices)
	{
		// Prevents memory leak
		if (m_buffer.Get() != nullptr)
			m_buffer.Reset();

		m_bufferSize = numOfVertices;
		if(m_stride.get() == nullptr)
			m_stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * numOfVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_buffer.GetAddressOf());
		return hr;
	}

private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	std::unique_ptr<UINT> m_stride = nullptr;
	UINT m_bufferSize = 0;
};