#pragma once
#include "ConstantBuffer.h"
#include "Shaders.h"
#include <wrl/client.h>
#include "../Util/objfilemodel.h"

using namespace DirectX;

class Light;
class DirectionalLight;
class PointLight;

class Model
{
public:
	Model() = default;
	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objPath, const char* texturePath, const char* vertexShader, const char* pixelShader, ConstantBuffer<CB_VS_Model>& cbVertexShader);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix, Light* ambientLight, DirectionalLight* directionalLight, PointLight* pointLight);

	bool LoadObjModel(const char* filename, const char* vertexShader, const char* pixelShader);
	bool AddTexture(const char* filename);

	ObjFileModel* GetObjFileModel();
private:
	ObjFileModel* m_objFileModel = nullptr;
	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
	ConstantBuffer<CB_VS_Model>* m_cbVertexShader = nullptr;
};

