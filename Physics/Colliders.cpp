#include "Colliders.h"
#include "../Entities/GameObject.h"

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

float SphereCollider::GetSphereColliderRadius(float scale)
{
	return m_radius * scale;
}

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
