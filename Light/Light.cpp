#include "Light.h"

Light::Light()
{
	m_colour = XMVectorZero();
}

Light::Light(const XMVECTOR& colour)
{
	m_colour = colour;
}

Light::Light(const XMFLOAT3& colour)
{
	m_colour = XMLoadFloat3(&colour);
}

Light::Light(const float r, const float g, const float b, const float a)
{
	m_colour = XMVectorSet(r, g, b, a);
}

Light::~Light()
{
}

XMVECTOR Light::GetColour()
{
	return m_colour;
}

void Light::SetColour(XMFLOAT3& newColour)
{
	m_colour = XMLoadFloat3(&newColour);
}

void Light::SetColour(XMVECTOR& newColour)
{
	m_colour = newColour;
}

void Light::Setcolour(float r, float g, float b, float a)
{
	m_colour = XMVectorSet(r, g, b, a);
}
