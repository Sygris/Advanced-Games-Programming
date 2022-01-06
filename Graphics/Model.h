#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shaders.h"
#include <wrl/client.h>
#include "../Util/objfilemodel.h"

using namespace DirectX;

class Model
{
public:
	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VertexShader>& cbVertexShader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& viewProjectionMatrix);

	bool LoadObjModel(char* filename);

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

	// Get Directional Vectors
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
private:
	void UpdateWorldMatrix();

	ObjFileModel* m_objFileModel;
	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	ConstantBuffer<CB_VertexShader>* m_cbVertexShader = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;

	VertexBuffer<Vertex> m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	XMMATRIX worldMatrix = XMMatrixIdentity();

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

	// Default Vectors
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
};

