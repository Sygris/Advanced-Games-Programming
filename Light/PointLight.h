#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(float range, XMFLOAT3 attenuation);

	float GetRange();
	const XMFLOAT3 GetAttenuation();
	const XMVECTOR GetInversePosition(const XMMATRIX& world);

	void SetRange(float range);
	void SetAttenuation(const XMFLOAT3& attenuation);

private:
	XMFLOAT3 m_attenuation;
	float m_range;
};

