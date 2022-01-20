#pragma once
#include <string>
#include <vector>
#include <d3d11.h>
#include "Graphics/ConstantBuffer.h"
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

using namespace DirectX;

class GameObject;

class Light;
class DirectionalLight;
class PointLight;

class Map
{
public:
	Map(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filePath, ConstantBuffer<CB_VS_Model>& cbVertexShader);
	~Map();

	void Draw(const XMMATRIX& viewProjectionMatrix, Light* ambientLight, DirectionalLight* directionalLight, PointLight* pointLight);

	std::vector<GameObject*> GetWalls();
	XMFLOAT3 GetPlayerStartPosition();
private:
	std::vector<std::string> m_mapLayout;
	std::vector<GameObject*> m_floor;
	std::vector<GameObject*> m_walls;
	XMFLOAT3 m_playerStartPosition;

	void LoadMapFromFile(std::string filePath);
	void InitializeMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader);

	void CreateFloor(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column);
	void CreateWall(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column);
	void CreateRock(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column, bool isHorizontal);

	void CreateHealthKit(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column);

};

