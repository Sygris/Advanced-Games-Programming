#pragma once

#include "../Entities/GameObject.h"

class Light : public GameObject
{
public:
	Light();
	Light(const XMVECTOR& colour);
	Light(const XMFLOAT3& colour);
	Light(const float r, const float g, const float b, const float a);

	~Light();

	XMVECTOR GetColour();

	void SetColour(XMFLOAT3& newColour);
	void SetColour(XMVECTOR& newColour);
	void SetColour(float r, float g, float b, float a);

protected:
	XMVECTOR m_colour;
};

