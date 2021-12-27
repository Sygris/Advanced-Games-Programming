#pragma once
#include <DirectXMath.h>

/// <summary>
/// Struct that represent 3D vertex
/// </summary>
struct Vertex
{
	Vertex() {};
	Vertex(float x, float y, float z, float u, float v)
		: m_position(x, y, z), m_texCoord(u, v) {}

	DirectX::XMFLOAT3 m_position{};
	DirectX::XMFLOAT2 m_texCoord{};
};