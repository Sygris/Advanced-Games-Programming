#include "Model.h"

bool Model::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
    m_device = device;
    m_deviceContext = deviceContext;
    m_cbVertexShader = &cbVertexShader;

	LoadObjModel(objPath);
	AddTexture(texturePath);

    return true;
}

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{
	XMMATRIX transpose;

	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

    // Update Constant Buffer with World View Projection Matrix
    m_cbVertexShader->data.worldViewProjection = worldMatrix * viewProjectionMatrix;
    m_cbVertexShader->data.worldViewProjection = XMMatrixTranspose(m_cbVertexShader->data.worldViewProjection);
	transpose = XMMatrixTranspose(worldMatrix);
	m_cbVertexShader->data.ambientLightColour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);
	m_cbVertexShader->data.directionalLightColour = XMVectorSet(0.5f, 0.25f, 0.5f, 0.0f);
	m_cbVertexShader->data.directionalLightVector = XMVector3Transform(XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f), transpose);
	m_cbVertexShader->data.directionalLightVector = XMVector3Normalize(m_cbVertexShader->data.directionalLightVector);
    m_cbVertexShader->ApplyChanges();

    m_deviceContext->VSSetConstantBuffers(0, 1, m_cbVertexShader->GetAddressOf());
    m_deviceContext->PSSetShaderResources(0, 1, &m_texture);

	m_objFileModel->Draw();
}

bool Model::LoadObjModel(const char* filename)
{
	//Load new model
	if (m_objFileModel == nullptr)
	{
		m_objFileModel = new ObjFileModel((char*)filename, m_device.Get(), m_deviceContext.Get());
		if (m_objFileModel->filename == "FILE NOT LOADED") 
			return S_FALSE;
	}

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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numOfLayout = ARRAYSIZE(iedesc);

	if (!m_vertexShader.Initialise(m_device.Get(), shaderFolder + L"ModelShader.cso", iedesc, numOfLayout))
		return false;

	if (!m_pixelShader.Initialise(m_device.Get(), shaderFolder + L"PixelShader.cso"))
		return false;

	return true;
}

bool Model::AddTexture(const char* filename)
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile(m_device.Get(), filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(hr))
		return false;

	return true;
}

ObjFileModel* Model::GetObjFileModel()
{
	return m_objFileModel;
}
