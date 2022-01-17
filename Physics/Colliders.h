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
	XMVECTOR GetSphereColliderWorldSpacePosition(const XMFLOAT3& scale, const XMFLOAT3& rotation, const XMVECTOR& position);

	float GetSphereColliderRadius(float scale = 1.0f);
};

class BoxCollider
{
public:
	XMFLOAT3 m_minimum = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_maximum = { 0.0f, 0.0f, 0.0f };

	void CalculateCollider(ObjFileModel* model);
};

bool SphereToSphereCollision(GameObject& a, GameObject& b);
bool BoxToBoxCollision(BoxCollider& a, BoxCollider& b);
bool BoxToSphereCollision(SphereCollider& sphere, BoxCollider& box);

// TODO: When using this I get an unresolved 
//bool SphereToSphereCollision(GameObject& a, GameObject& b, XMVECTOR& newPosition);