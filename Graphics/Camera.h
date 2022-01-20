#pragma once
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
#include "../Physics/Colliders.h"

// I decided to use the namespace to speed up and
// avoid having to write DirectX:: multiple times
using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjectMatrix(float fovDegrees, float aspectRation, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	// Get Position and Rotation (Returns a vector or a float3)
	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	// Set Position (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void SetPosition(const XMVECTOR& position);
	void SetPosition(const XMFLOAT3& position);
	void SetPosition(float x, float y, float z);
	
	// Set Rotation (Can use XMVECTOR, XMFLOAT3 or each individual component)
	void SetRotation(const XMVECTOR& rotation);
	void SetRotation(const XMFLOAT3& rotation);
	void SetRotation(float pitch, float yaw, float roll);

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

	// Get Collider
	SphereCollider* GetCollider();

	// Get Directional Vectors
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
private:
	void UpdateViewMatrix();

	// VECTORS
	XMVECTOR m_positionVector;
	XMVECTOR m_rotationVector;

	// Directional Vectors
	XMVECTOR m_forwardVector;
	XMVECTOR m_leftVector;
	XMVECTOR m_rightVector;
	XMVECTOR m_backwardVector;
	
	// FLOAT3
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;

	// MATRICES
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;

	// Collider
	SphereCollider m_collider;

	// Default Vectors
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
};

