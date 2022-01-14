#pragma once
#include <wrl/client.h>
#include "../Graphics/Model.h"

class GameObject
{
public:
	~GameObject();

	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, ConstantBuffer<CB_VS_Model>& cbVertexShader);
	void Draw(const XMMATRIX& viewProjectionMatrix);

	// Get Position, Rotation and Scale (Returns a vector or a float3)
	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;
	const XMVECTOR& GetScaleVector() const;
	const XMFLOAT3& GetScaleFloat3() const;

	// Set Position (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void SetPosition(const XMVECTOR& position);
	void SetPosition(const XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// Set Rotation (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void SetRotation(const XMVECTOR& rotation);
	void SetRotation(const XMFLOAT3& rotation);
	void SetRotation(float pitch, float yaw, float roll);

	// Set Scale (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void SetScale(const XMVECTOR& scale);
	void SetScale(const XMFLOAT3& scale);
	void SetScale(float x, float y, float z);

	// Adjust Position (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void AdjustPosition(const XMVECTOR& position);
	void AdjustPosition(const XMFLOAT3& position);
	void AdjustPosition(float x, float y, float z);

	// Adjust Rotation (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void AdjustRotation(const XMVECTOR& rotation);
	void AdjustRotation(const XMFLOAT3& rotation);
	void AdjustRotation(float pitch, float yaw, float roll);

	// Set Look At position
	void SetLookAtPos(XMFLOAT3 lookAtPosition);

	// Get Directional Vectors
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
protected:
	XMMATRIX worldMatrix = XMMatrixIdentity();

	// VECTORS
	XMVECTOR m_positionVector;
	XMVECTOR m_rotationVector;
	XMVECTOR m_scaleVector;

	// FLOAT3
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	// Directional Vectors
	XMVECTOR m_forwardVector;
	XMVECTOR m_leftVector;
	XMVECTOR m_rightVector;
	XMVECTOR m_backwardVector;

	// Default Vectors
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
private:
	void UpdateWorldMatrix();

	Model* m_model = nullptr;
};