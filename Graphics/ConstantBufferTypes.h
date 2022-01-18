#pragma once
#include <DirectXMath.h>

#pragma region Vertex Shader - Constant Buffers
// Basic CB for Vertex Shader
struct CB_VertexShader
{
	DirectX::XMMATRIX mat;
};

// Model's Constant Buffer
struct CB_VS_Model
{
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMVECTOR directionalLightVector;
	DirectX::XMVECTOR directionalLightColour;
	DirectX::XMVECTOR ambientLightColour;
	DirectX::XMVECTOR pointLightPosisiton;
	DirectX::XMVECTOR pointLightColour;
	DirectX::XMFLOAT3 pointLightAttenuation;
	float range;
};
#pragma endregion

#pragma region Pixel Shader - Constant Buffer
struct CB_PixelShader
{
	float alpha = 1.0f;
};
#pragma endregion
