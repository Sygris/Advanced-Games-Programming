#pragma once
#include <DirectXMath.h>

// Constant Buffer Vertex Shader
struct CB_VertexShader
{
	DirectX::XMMATRIX mat;
};

// Constant Buffer Pixel Shader
struct CB_PixelShader
{
	float alpha = 1.0f;
};