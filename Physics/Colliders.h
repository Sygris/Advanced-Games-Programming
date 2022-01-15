#pragma once
#include "../Util/objfilemodel.h"

class SphereCollider
{
public:
	XMFLOAT3 m_colliderCentre;
	float m_radius;

	void CalculateModelCentrePoint(ObjFileModel* model);
	void CalculateSphereColliderRadius(ObjFileModel* model, float scale = 1.0f);
};

