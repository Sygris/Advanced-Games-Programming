#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
    m_direction = XMVectorZero();
}

XMVECTOR DirectionalLight::GetDirection(XMMATRIX world)
{
    XMMATRIX transpose = XMMatrixTranspose(world);
    XMMATRIX rotateLight = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR newDirection = XMVector3Normalize(XMVector3Transform(XMVector3Transform(m_direction, rotateLight), transpose));

    return newDirection;
}

void DirectionalLight::SetDirection(const XMVECTOR& direction)
{
    m_direction = direction;
}

void DirectionalLight::SetDirection(const XMFLOAT3& direction)
{
    m_direction = XMLoadFloat3(&direction);
}

void DirectionalLight::SetDirection(const float x, float y, float z)
{
    m_direction = XMVectorSet(x, y, z, 1.0f);
}
