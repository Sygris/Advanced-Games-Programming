#include "Model.h"
#include "../HelperFunctions.h"
#include "..\Light\Light.h"
#include "..\Light\DirectionalLight.h"
#include "..\Light\PointLight.h"

bool Model::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, const char* vertexShader, const char* pixelShader, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_cbVertexShader = &cbVertexShader;

	LoadObjModel(objPath, vertexShader, pixelShader);
	AddTexture(texturePath);

	return true;
}

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix, Light* ambientLight, DirectionalLight* directionalLight, PointLight* pointLight)
{
	XMMATRIX transpose;

	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

	// Update Constant Buffer with World View Projection Matrix
	m_cbVertexShader->data.worldViewProjection = worldMatrix * viewProjectionMatrix;
	m_cbVertexShader->data.worldViewProjection = XMMatrixTranspose(m_cbVertexShader->data.worldViewProjection);
	transpose = XMMatrixTranspose(worldMatrix);
	m_cbVertexShader->data.ambientLightColour = ambientLight->GetColour();
	m_cbVertexShader->data.directionalLightColour = directionalLight->GetColour();
	m_cbVertexShader->data.directionalLightVector = XMVector3Transform(directionalLight->GetDirection(worldMatrix), transpose);
	m_cbVertexShader->data.directionalLightVector = XMVector3Normalize(m_cbVertexShader->data.directionalLightVector);

	if (pointLight != nullptr)
	{
		m_cbVertexShader->data.pointLightAttenuation = pointLight->GetAttenuation();
		m_cbVertexShader->data.pointLightColour = pointLight->GetColour();
		m_cbVertexShader->data.pointLightPosisiton = pointLight->GetInversePosition(worldMatrix);
		m_cbVertexShader->data.range = pointLight->GetRange();
	}

	m_cbVertexShader->ApplyChanges();

	m_deviceContext->VSSetConstantBuffers(0, 1, m_cbVertexShader->GetAddressOf());
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture);

	m_objFileModel->Draw();
}

bool Model::LoadObjModel(const char* filename, const char* vertexShader, const char* pixelShader)
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
	std::wstring path = L"";

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


	HelperFunctions::StringToWide(path, vertexShader);
	if (!m_vertexShader.Initialise(m_device.Get(), shaderFolder + path, iedesc, numOfLayout))
		return false;

	HelperFunctions::StringToWide(path, pixelShader);
	if (!m_pixelShader.Initialise(m_device.Get(), shaderFolder + path))
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
