#pragma once
#include <string>
#include <vector>
#include <d3d11.h>
#include "Graphics/ConstantBuffer.h"

class GameObject;

class Map
{
public:
	Map(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filePath, ConstantBuffer<CB_VS_Model>& cbVertexShader);

	void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
private:
	std::vector<std::string> m_mapLayout;
	std::vector<GameObject*> m_floor;
	std::vector<GameObject*> m_wall;
	GameObject* m_statue;

	void LoadMapFromFile(std::string filePath);
	void InitializeMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader);
};

