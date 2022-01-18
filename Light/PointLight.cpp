#include "PointLight.h"

PointLight::PointLight()
{
	Light(1.0f, 0.0f, 0.0f, 1.0f);
	m_range = 8;
	m_attenuation = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

PointLight::PointLight(float range, XMFLOAT3 attenuation)
{
	Light(1.0f, 0.0f, 0.0f, 1.0f);
	m_range = range;
	m_attenuation = attenuation;
}

float PointLight::GetRange()
{
	return m_range;
}

const XMFLOAT3 PointLight::GetAttenuation()
{
	return m_attenuation;
}

const XMVECTOR PointLight::GetInversePosition(const XMMATRIX& world)
{
	XMVECTOR determinant; // Inverse function returns determinant but isn't used 
	XMMATRIX inverse = XMMatrixInverse(&determinant, world); // Inverse matrix

	return XMVector3Transform(XMLoadFloat3(&m_position), inverse);
}

void PointLight::SetRange(float range)
{
	m_range = range;
}

void PointLight::SetAttenuation(const XMFLOAT3& attenuation)
{
	m_attenuation = attenuation;
}
