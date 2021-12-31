#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h> // ComPtr
#include <string>

class VertexShader
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numOfElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

class PixelShader
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer = nullptr;
};