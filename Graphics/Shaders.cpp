#include "Shaders.h"

#pragma region Vertex Shader
bool VertexShader::Initialise(ID3D11Device*& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numOfElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &m_shaderBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to load Vertex Shader\n");

		return false;
	}

	hr = device->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Vertex Shader");
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numOfElements, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), &m_inputLayout);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create input layout!");
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
	return m_shaderBuffer;
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return m_inputLayout;
}
#pragma endregion

#pragma region Pixel Shader
bool PixelShader::Initialise(ID3D11Device*& device, std::wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &m_shaderBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to load Pixel Shader\n");

		return false;
	}

	hr = device->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Pixel Shader");
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return m_pixelShader;
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return m_shaderBuffer;
}
#pragma endregion

