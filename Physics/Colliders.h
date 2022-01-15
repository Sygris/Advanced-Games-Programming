#pragma once
#include "../Util/objfilemodel.h"

class GameObject;

class SphereCollider
{
public:
	XMFLOAT3 m_colliderCentre;
	float m_radius;

	void CalculateModelCentrePoint(ObjFileModel* model);
	void CalculateSphereColliderRadius(ObjFileModel* model);

	XMVECTOR GetSphereColliderWorldSpacePosition(const XMMATRIX& worldMatrix);
	float GetSphereColliderRadius(float scale = 1.0f);
};

bool SphereToSphereCollision(GameObject& a, GameObject& b);