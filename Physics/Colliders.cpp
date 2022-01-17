#include "Colliders.h"
#include "../Entities/GameObject.h"

#pragma region Sphere Collider
void SphereCollider::CalculateModelCentrePoint(ObjFileModel* model)
{
	XMFLOAT3 minimum = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 maximum = { 0.0f, 0.0f, 0.0f };

	for (size_t i = 0; i < model->numverts; i++)
	{
		// X axis
		if (model->vertices[i].Pos.x > maximum.x)
			maximum.x = model->vertices[i].Pos.x;
		if (model->vertices[i].Pos.x < minimum.x)
			minimum.x = model->vertices[i].Pos.x;

		// Y axis
		if (model->vertices[i].Pos.y > maximum.y)
			maximum.y = model->vertices[i].Pos.y;
		if (model->vertices[i].Pos.y < minimum.y)
			minimum.y = model->vertices[i].Pos.y;

		// Z axis
		if (model->vertices[i].Pos.z > maximum.z)
			maximum.z = model->vertices[i].Pos.z;
		if (model->vertices[i].Pos.z < minimum.z)
			minimum.z = model->vertices[i].Pos.z;
	}

	m_colliderCentre = XMFLOAT3((minimum.x + (maximum.x - minimum.x) / 2), (minimum.y + (maximum.y - minimum.y) / 2), (minimum.z + (maximum.z - minimum.z) / 2));
}

void SphereCollider::CalculateSphereColliderRadius(ObjFileModel* model)
{
	for (size_t i = 0; i < model->numverts; i++)
	{
		float radius = (float)sqrtf(powf((m_colliderCentre.x - model->vertices[i].Pos.x), 2) + powf((m_colliderCentre.y - model->vertices[i].Pos.y), 2) + powf((m_colliderCentre.z - model->vertices[i].Pos.z), 2));

		if (radius > m_radius)
			m_radius = radius;
	}
}

XMVECTOR SphereCollider::GetSphereColliderWorldSpacePosition(const XMMATRIX& worldMatrix)
{
	XMVECTOR offset = XMLoadFloat3(&m_colliderCentre);
	offset = XMVector3Transform(offset, worldMatrix);

	return offset;
}

XMVECTOR SphereCollider::GetSphereColliderWorldSpacePosition(const XMFLOAT3& scale, const XMFLOAT3& rotation, const XMVECTOR& position)
{
	XMMATRIX offsetScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX offsetRotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX offsetPosition = XMMatrixTranslationFromVector(position);

	XMMATRIX world = offsetScale * offsetRotation * offsetPosition;

	XMVECTOR offset = XMLoadFloat3(&m_colliderCentre);
	offset = XMVector3Transform(offset, world);

	return offset;
}

float SphereCollider::GetSphereColliderRadius(float scale)
{
	return m_radius * scale;
}
#pragma endregion

#pragma region Box Collider
void BoxCollider::CalculateCollider(ObjFileModel* model)
{
	for (unsigned int i = 0; i < model->numverts; i++)
	{
		//X
		if (model->vertices[i].Pos.x > m_maximum.x)
			m_maximum.x = model->vertices[i].Pos.x;
		if (model->vertices[i].Pos.x < m_minimum.x)
			m_minimum.x = model->vertices[i].Pos.x;

		//Y
		if (model->vertices[i].Pos.y > m_maximum.y)
			m_maximum.y = model->vertices[i].Pos.y;
		if (model->vertices[i].Pos.y < m_minimum.y)
			m_minimum.y = model->vertices[i].Pos.y;

		//Z
		if (model->vertices[i].Pos.z > m_maximum.z)
			m_maximum.z = model->vertices[i].Pos.z;
		if (model->vertices[i].Pos.z < m_minimum.z)
			m_minimum.z = model->vertices[i].Pos.z;
	}
}
#pragma endregion

bool SphereToSphereCollision(GameObject& a, GameObject& b)
{
	XMVECTOR worldPositionA = a.GetSphereCollider()->GetSphereColliderWorldSpacePosition(a.GetWorldMatrix());
	XMVECTOR worldPositionB = b.GetSphereCollider()->GetSphereColliderWorldSpacePosition(b.GetWorldMatrix());

	float x = (XMVectorGetX(worldPositionA) - XMVectorGetX(worldPositionB)) * (XMVectorGetX(worldPositionA) - XMVectorGetX(worldPositionB));
	float y = (XMVectorGetY(worldPositionA) - XMVectorGetY(worldPositionB)) * (XMVectorGetY(worldPositionA) - XMVectorGetY(worldPositionB));
	float z = (XMVectorGetZ(worldPositionA) - XMVectorGetZ(worldPositionB)) * (XMVectorGetZ(worldPositionA) - XMVectorGetZ(worldPositionB));

	float distance = sqrtf(x + y + z);

	return distance < (a.GetSphereCollider()->m_radius + b.GetSphereCollider()->m_radius);
}

bool BoxToBoxCollision(BoxCollider& a, BoxCollider& b)
{
	return (a.m_minimum.x <= b.m_maximum.x && a.m_maximum.x >= b.m_minimum.x) &&
		(a.m_minimum.y <= b.m_maximum.y && a.m_maximum.y >= b.m_minimum.y) &&
		(a.m_minimum.z <= b.m_maximum.z && a.m_maximum.z >= b.m_minimum.z);
}

bool BoxToSphereCollision(SphereCollider& sphere, BoxCollider& box)
{
	
	return false;
}

#pragma region TODO : When using this I get an unresolved linking error. Fixing this will prevent having to move then check collision and if colliding having to move back
//bool SphereToSphereCollision(GameObject& a, GameObject& b, XMVECTOR& newPosition)
//{
//	XMVECTOR worldPositionA = a.GetSphereCollider()->GetSphereColliderWorldSpacePosition(a.GetScaleFloat3(), a.GetRotationFloat3(), newPosition);
//	XMVECTOR worldPositionB = b.GetSphereCollider()->GetSphereColliderWorldSpacePosition(b.GetWorldMatrix());
//
//	float x = (XMVectorGetX(worldPositionA) - XMVectorGetX(worldPositionB)) * (XMVectorGetX(worldPositionA) - XMVectorGetX(worldPositionB));
//	float y = (XMVectorGetY(worldPositionA) - XMVectorGetY(worldPositionB)) * (XMVectorGetY(worldPositionA) - XMVectorGetY(worldPositionB));
//	float z = (XMVectorGetZ(worldPositionA) - XMVectorGetZ(worldPositionB)) * (XMVectorGetZ(worldPositionA) - XMVectorGetZ(worldPositionB));
//
//	float distance = sqrtf(x + y + z);
//
//	return distance < (a.GetSphereCollider()->m_radius + b.GetSphereCollider()->m_radius);
//}
#pragma endregion