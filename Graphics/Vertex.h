#pragma once
#include <DirectXMath.h>

/// <summary>
/// Struct that represent 2D and 3D vertex
/// </summary>
struct Vertex
{
	Vertex() {};
	Vertex(float x, float y)
		: m_position(x, y) {}

	DirectX::XMFLOAT2 m_position;
};