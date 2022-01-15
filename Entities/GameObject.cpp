#include "GameObject.h"

GameObject::GameObject()
{
    m_model = new Model();
}

GameObject::~GameObject()
{
    if (m_model != nullptr)
    {
        delete m_model;
        m_model = nullptr;
    }
}

bool GameObject::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
    m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_positionVector = XMVectorZero();
    m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rotationVector = XMVectorZero();
    m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_scaleVector = XMLoadFloat3(&m_scale);
    
    if (!m_model->Initialise(device, deviceContext, objPath, texturePath, cbVertexShader))
        return false;

    m_collider.CalculateModelCentrePoint(m_model->GetObjFileModel());
    m_collider.CalculateSphereColliderRadius(m_model->GetObjFileModel());

    return true;
}

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
    m_model->Draw(m_worldMatrix, viewProjectionMatrix);
}

const XMVECTOR& GameObject::GetPositionVector() const
{
    return m_positionVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const
{
    return m_position;
}

const XMVECTOR& GameObject::GetRotationVector() const
{
    return m_rotationVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const
{
    return m_rotation;
}

const XMVECTOR& GameObject::GetScaleVector() const
{
    return m_scaleVector;
}

const XMFLOAT3& GameObject::GetScaleFloat3() const
{
    return m_scale;
}

const XMMATRIX& GameObject::GetWorldMatrix() const
{
    return m_worldMatrix;
}

SphereCollider* GameObject::GetSphereCollider()
{
    return &m_collider;
}

void GameObject::SetPosition(const XMVECTOR& position)
{
    XMStoreFloat3(&m_position, position);
    m_positionVector = position;

    UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3& position)
{
    m_position = position;
    m_positionVector = XMLoadFloat3(&m_position);

    UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
    m_position = XMFLOAT3(x, y, z);
    m_positionVector = XMLoadFloat3(&m_position);

    UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMVECTOR& rotation)
{
    XMStoreFloat3(&m_rotation, rotation);
    m_rotationVector = rotation;

    UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3& rotation)
{
    m_rotation = rotation;
    m_rotationVector = XMLoadFloat3(&m_rotation);

    UpdateWorldMatrix();
}

void GameObject::SetRotation(float pitch, float yaw, float roll)
{
    m_rotation = XMFLOAT3(pitch, yaw, roll);
    m_rotationVector = XMLoadFloat3(&m_rotation);

    UpdateWorldMatrix();
}

void GameObject::SetScale(const XMVECTOR& scale)
{
    XMStoreFloat3(&m_scale, scale);
    m_scaleVector = scale;

    UpdateWorldMatrix();
}

void GameObject::SetScale(const XMFLOAT3& scale)
{
    m_scale = scale;
    m_scaleVector = XMLoadFloat3(&scale);

    UpdateWorldMatrix();
}

void GameObject::SetScale(float x, float y, float z)
{
    m_scale = XMFLOAT3(x, y, z);
    m_scaleVector = XMLoadFloat3(&m_scale);

    UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR& position)
{
    m_positionVector += position;
    XMStoreFloat3(&m_position, m_positionVector);

    UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3& position)
{
    m_position.x += position.x;
    m_position.y += position.y;
    m_position.z += position.z;
    m_positionVector = XMLoadFloat3(&m_position);

    UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
    m_position.x += x;
    m_position.y += y;
    m_position.z += z;
    m_positionVector = XMLoadFloat3(&m_position);

    UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR& rotation)
{
    m_rotationVector += rotation;
    XMStoreFloat3(&m_rotation, m_rotationVector);

    UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3& rotation)
{
    m_rotation.x += rotation.x;
    m_rotation.y += rotation.y;
    m_rotation.z += rotation.z;
    m_rotationVector = XMLoadFloat3(&m_rotation);

    UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float pitch, float yaw, float roll)
{
    m_rotation.x += pitch;
    m_rotation.y += yaw;
    m_rotation.z += roll;
    m_rotationVector = XMLoadFloat3(&m_rotation);

    UpdateWorldMatrix();
}

void GameObject::SetLookAtPos(XMFLOAT3 lookAtPosition)
{
    // If lookAtPosition is the same as the camera return since that should be impossible
    if (lookAtPosition.x == m_position.x && lookAtPosition.y == m_position.y && lookAtPosition.z == m_position.z)
        return;

    lookAtPosition.x = m_position.x - lookAtPosition.x;
    lookAtPosition.y = m_position.y - lookAtPosition.y;
    lookAtPosition.z = m_position.z - lookAtPosition.z;

    float pitch = 0.0f;
    // As long as the y position the camera needs to look at is not the same y value
    // Calculate the distance and set pitch to atan(y / distance)
    if (lookAtPosition.y != 0.0f)
    {
        // Prevents warning 
        double x = static_cast<double>(lookAtPosition.x);
        double y = static_cast<double>(lookAtPosition.y);

        const float distance = sqrt(x * x + y * y);
        pitch = atan(lookAtPosition.y / distance);
    }

    float yaw = 0.0f;
    // Avoids dividing zero by zero or something weird
    if (lookAtPosition.x != 0.0f)
        yaw = atan(lookAtPosition.x / lookAtPosition.z);

    // If bigger than zero add pi to rotate around
    if (lookAtPosition.z > 0)
        yaw += XM_PI;

    SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR& GameObject::GetForwardVector()
{
    return m_forwardVector;
}

const XMVECTOR& GameObject::GetRightVector()
{
    return m_rightVector;
}

const XMVECTOR& GameObject::GetBackwardVector()
{
    return m_backwardVector;
}

const XMVECTOR& GameObject::GetLeftVector()
{
    return m_leftVector;
}

void GameObject::UpdateWorldMatrix()
{
    // Update World Matrix of the GameObject
    m_worldMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    m_worldMatrix *= XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    m_worldMatrix *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

    // Updates Directional Vectors of the GameObject
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, m_rotation.y, 0.0f);
    m_forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotationMatrix);
    m_backwardVector = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, rotationMatrix);
    m_leftVector = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, rotationMatrix);
    m_rightVector = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, rotationMatrix);
}
