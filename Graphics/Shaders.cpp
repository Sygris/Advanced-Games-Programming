#include "Shaders.h"

bool VertexShader::Initialise(ID3D11Device*& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numOfElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &m_vertexBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to load Vertex Shader\n");
		
		return false;
	}

	hr = device->CreateVertexShader(m_vertexBuffer->GetBufferPointer(), m_vertexBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
	{
		OutputDebugString("Faield to create Vertex Shader");
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numOfElements, m_vertexBuffer->GetBufferPointer(), m_vertexBuffer->GetBufferSize(), &m_inputLayout);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to get pointer to back buffer!");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return m_vertexShader;
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return m_vertexBuffer;
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return m_inputLayout;
}

