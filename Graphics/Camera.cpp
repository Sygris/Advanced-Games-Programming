#include "Camera.h"

Camera::Camera()
{
	// Sets the postion to (o, 0, 0) and then transforms the XMFLOAT3 into a vector
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_positionVector = XMLoadFloat3(&m_position);

	// Does the same thing as above
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateViewMatrix();
}

void Camera::SetProjectMatrix(float fovDegrees, float aspectRation, float nearZ, float farZ)
{
	// Transforms the FOV from degrees to radians
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRation, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return m_positionVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return m_position;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return m_rotationVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return m_rotation;
}

void Camera::SetPosition(const XMVECTOR& position)
{
	XMStoreFloat3(&m_position, position);
	m_positionVector = position;

	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
	m_positionVector = XMLoadFloat3(&m_position);

	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rotation)
{
	XMStoreFloat3(&m_rotation, rotation);
	m_rotationVector = rotation;

	UpdateViewMatrix();
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
	m_rotation = XMFLOAT3(pitch, yaw, roll);
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& position)
{
	m_positionVector += position;
	XMStoreFloat3(&m_position, m_positionVector);

	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_positionVector = XMLoadFloat3(&m_position);

	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rotation)
{
	m_rotationVector += rotation;
	XMStoreFloat3(&m_rotation, m_rotationVector);

	UpdateViewMatrix();
}

void Camera::AdjustRotation(float pitch, float yaw, float roll)
{
	m_rotation.x += pitch;
	m_rotation.y += yaw;
	m_rotation.z += roll;
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPosition)
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

const XMVECTOR& Camera::GetForwardVector()
{
	return m_forwardVector;
}

const XMVECTOR& Camera::GetRightVector()
{
	return m_rightVector;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return m_backwardVector;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return m_leftVector;
}

// Updates View Matrix and the movement vectors
void Camera::UpdateViewMatrix()
{
	// Calculates camera's target by using its rotation and forward vector

	// Calculate camera Rotation Matrix
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	// Calculate unit vector of camera target based off camera forward value transformed by cameraRotationMatrix
	XMVECTOR cameraTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, cameraRotationMatrix);
	// Adjust cameraTarget to be offset by the camera's current position
	cameraTarget += m_positionVector;
	// Calculate the up direction based on the current rotation
	XMVECTOR upDirection = XMVector3TransformCoord(DEFAULT_UP_VECTOR, cameraRotationMatrix);

	// Rebuild the view matrix
	m_viewMatrix = XMMatrixLookAtLH(m_positionVector, cameraTarget, upDirection);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, m_rotation.y, 0.0f);
	m_forwardVector = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	m_backwardVector = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	m_leftVector = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	m_rightVector = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
