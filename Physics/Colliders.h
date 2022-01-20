#pragma once
#include "../Util/objfilemodel.h"

class GameObject;
class Camera;

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

	XMVECTOR m_worldPositionMinimum = XMVectorZero();
	XMVECTOR m_worldPositionMaximum = XMVectorZero();

	void CalculateCollider(ObjFileModel* model);
	void CalculateBoundingBoxWorldPosition(XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 position);
};

bool SphereToSphereCollision(GameObject* a, GameObject* b);
bool CameraToSphereCollision(Camera* a, GameObject* b);
bool SphereToSphereCollision(GameObject* a, GameObject* b, XMVECTOR& newPosition);
bool BoxToBoxCollision(BoxCollider& a, BoxCollider& b);
bool BoxToSphereCollision(GameObject* a, GameObject* box);
bool BoxToSphereCollision(GameObject* a, GameObject* box, XMVECTOR& newPosition);
bool BoxToSphereCollision(SphereCollider* a, GameObject* box, XMVECTOR& newPosition);