#include "Model.h"

bool Model::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VertexShader>& cbVertexShader)
{
    m_device = device;
    m_deviceContext = deviceContext;
    m_texture = texture;
    m_cbVertexShader = &cbVertexShader;

#pragma region Initialise Vertex Buffer
    //Textured Square
    Vertex v[] =
    {
        Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f), //FRONT Bottom Left   - [0]
        Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f), //FRONT Top Left      - [1]
        Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f),  //FRONT Top Right     - [2]
        Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f),  //FRONT Bottom Right   - [3]
        Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f),  //BACK Bottom Left   - [4]
        Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f),  //BACK Top Left      - [5]
        Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f),   //BACK Top Right     - [6]
        Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f),   //BACK Bottom Right   - [7]
    };

    //Load Vertex Data
    HRESULT hr = m_vertexBuffer.Initialise(m_device.Get(), v, ARRAYSIZE(v));
    if (FAILED(hr))
    {
        OutputDebugString("FAILED INITIALISING MODEL!\n");
        return false;
    }

    DWORD indices[] =
    {
        0, 1, 2, //FRONT
        0, 2, 3, //FRONT
        4, 7, 6, //BACK 
        4, 6, 5, //BACK
        3, 2, 6, //RIGHT SIDE
        3, 6, 7, //RIGHT SIDE
        4, 5, 1, //LEFT SIDE
        4, 1, 0, //LEFT SIDE
        1, 5, 6, //TOP
        1, 6, 2, //TOP
        0, 3, 7, //BOTTOM
        0, 7, 4, //BOTTOM
    };

#pragma endregion 

#pragma region Initialise Index Buffer
    //Load Index Data
    hr = m_indexBuffer.Initialize(m_device.Get(), indices, ARRAYSIZE(indices));
    if (FAILED(hr))
    {
        OutputDebugString("FAILED INITIALISING MODEL!\n");
        return false;
    }
#pragma endregion 

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);

    UpdateWorldMatrix();
    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
    m_texture = texture;
}

void Model::Draw(const XMMATRIX& viewProjectionMatrix)
{
    // Update Constant Buffer with World View Projection Matrix
    m_cbVertexShader->data.mat = worldMatrix * viewProjectionMatrix;
    m_cbVertexShader->data.mat = XMMatrixTranspose(m_cbVertexShader->data.mat);
    m_cbVertexShader->ApplyChanges();
    m_deviceContext->VSSetConstantBuffers(0, 1, m_cbVertexShader->GetAddressOf());

    m_deviceContext->PSSetShaderResources(0, 1, &m_texture);
    m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &offset);
    m_deviceContext->DrawIndexed(m_indexBuffer.BufferSize(), 0, 0);
}

bool Model::LoadObjModel(char* filename)
{
#pragma region Determine Shader Path (Change it to a better solution in the future)
	std::wstring shaderFolder = L"";

	// If running from visual studio
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 //x64
		shaderFolder = L"x64\\Debug\\";
#else //x86 (Win32)
		shaderFolder = L"Debug\\";
#endif
#else // Release Mode
#ifdef _WIN64 //x64
		shaderFolder = L"x64\\Release\\";
#else //x86 (Win32)
		shaderFolder = L"Release\\";
#endif
#endif // DEBUG
	}
#pragma endregion

	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//Be very careful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	UINT numOfLayout = ARRAYSIZE(iedesc);

	if (!m_vertexShader.Initialise(m_device, shaderFolder + L"ModelShader.cso", iedesc, numOfLayout))
		return false;

	if (!m_pixelShader.Initialise(m_device, shaderFolder + L"PixelShader.cso"))
		return false;

	return true;
}

const XMVECTOR& Model::GetPositionVector() const
{
	return m_positionVector;
}

const XMFLOAT3& Model::GetPositionFloat3() const
{
	return m_position;
}

const XMVECTOR& Model::GetRotationVector() const
{
	return m_rotationVector;
}

const XMFLOAT3& Model::GetRotationFloat3() const
{
	return m_rotation;
}

void Model::SetPosition(const XMVECTOR& position)
{
	XMStoreFloat3(&m_position, position);
	m_positionVector = position;

	UpdateWorldMatrix();
}

void Model::SetPosition(const XMFLOAT3& position)
{
	m_position = position;
	m_positionVector = XMLoadFloat3(&m_position);

	UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
	m_positionVector = XMLoadFloat3(&m_position);

	UpdateWorldMatrix();
}

void Model::SetRotation(const XMVECTOR& rotation)
{
	XMStoreFloat3(&m_rotation, rotation);
	m_rotationVector = rotation;

	UpdateWorldMatrix();
}

void Model::SetRotation(const XMFLOAT3& rotation)
{
	m_rotation = rotation;
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateWorldMatrix();
}

void Model::SetRotation(float pitch, float yaw, float roll)
{
	m_rotation = XMFLOAT3(pitch, yaw, roll);
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMVECTOR& position)
{
	m_positionVector += position;
	XMStoreFloat3(&m_position, m_positionVector);

	UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMFLOAT3& position)
{
	m_position.x += position.x;
	m_position.y += position.y;
	m_position.z += position.z;
	m_positionVector = XMLoadFloat3(&m_position);

	UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_positionVector = XMLoadFloat3(&m_position);

	UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMVECTOR& rotation)
{
	m_rotationVector += rotation;
	XMStoreFloat3(&m_rotation, m_rotationVector);

	UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMFLOAT3& rotation)
{
	m_rotation.x += rotation.x;
	m_rotation.y += rotation.y;
	m_rotation.z += rotation.z;
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateWorldMatrix();
}

void Model::AdjustRotation(float pitch, float yaw, float roll)
{
	m_rotation.x += pitch;
	m_rotation.y += yaw;
	m_rotation.z += roll;
	m_rotationVector = XMLoadFloat3(&m_rotation);

	UpdateWorldMatrix();
}

void Model::SetLookAtPos(XMFLOAT3 lookAtPosition)
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

const XMVECTOR& Model::GetForwardVector()
{
	return m_forwardVector;
}

const XMVECTOR& Model::GetRightVector()
{
	return m_rightVector;
}

const XMVECTOR& Model::GetBackwardVector()
{
	return m_backwardVector;
}

const XMVECTOR& Model::GetLeftVector()
{
	return m_leftVector;
}

void Model::UpdateWorldMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) * XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, m_rotation.y, 0.0f);
	m_forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotationMatrix);
	m_backwardVector = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, rotationMatrix);
	m_leftVector = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, rotationMatrix);
	m_rightVector = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, rotationMatrix);
}
