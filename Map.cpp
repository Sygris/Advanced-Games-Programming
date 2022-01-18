#include "Map.h"
#include <fstream>
#include "Entities/GameObject.h"

Map::Map(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filePath, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
	LoadMapFromFile(filePath);
	InitializeMap(device, deviceContext, cbVertexShader);
}

void Map::Draw(const XMMATRIX& viewProjectionMatrix, Light* ambientLight, DirectionalLight* directionalLight, PointLight* pointLight)
{
	for (size_t i = 0; i < m_floor.size(); i++)
	{
		m_floor[i]->Draw(viewProjectionMatrix, ambientLight, directionalLight, pointLight);
	}

	for (size_t j = 0; j < m_wall.size(); j++)
	{
		m_wall[j]->Draw(viewProjectionMatrix, ambientLight, directionalLight, pointLight);
	}
}

void Map::LoadMapFromFile(std::string filePath)
{
	std::string line;
	std::ifstream file(filePath.c_str());

	while (file >> line)
	{
		m_mapLayout.push_back(line);
	}

	file.close();

}

void Map::InitializeMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
	for (int i = 0; i < m_mapLayout.size(); i++)
	{
		for (int j = 0; j < m_mapLayout[i].size(); j++)
		{
			switch (m_mapLayout[i][j])
			{
			case '0':
			{
				GameObject* floor = new GameObject();
				floor->Initialise(device, deviceContext, "Assets/Models/cube.obj", "Assets/Textures/seamless_pavement.jpg", "ModelShader.cso", "PixelShader.cso", cbVertexShader);
				floor->SetPosition(i * 2, 0.0f, j * 1);
				m_floor.push_back(floor);
				break;
			}
			case '1':
			{
				GameObject* wall = new GameObject();
				wall->SetPosition(i * 2, 6.0f, j * 1);
				wall->SetScale(1.0f, 5.0f, 1.0f);
				wall->Initialise(device, deviceContext, "Assets/Models/cube.obj", "Assets/Textures/Wall1.png", "ModelShader.cso", "PixelShader.cso", cbVertexShader);
				m_wall.push_back(wall);
				break;
			}
			}
		}
	}
}
