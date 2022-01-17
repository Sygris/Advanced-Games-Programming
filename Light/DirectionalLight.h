#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	XMVECTOR GetDirection(XMMATRIX world);

	void SetDirection(const XMVECTOR& direction);
	void SetDirection(const XMFLOAT3& direction);
	void SetDirection(const float x, float y, float z);

private:
	XMVECTOR m_direction;
};

