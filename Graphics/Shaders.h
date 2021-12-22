#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

class VertexShader
{
public:
	bool Initialise(ID3D11Device*& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numOfElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D10Blob* m_vertexBuffer = nullptr;
	ID3D11InputLayout* m_inputLayout;
};