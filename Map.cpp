#include "Map.h"
#include <fstream>
#include "Entities/Rock.h"
#include "Entities\HealthKit.h"

Map::Map(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filePath, ConstantBuffer<CB_VS_Model>& cbVertexShader)
{
	LoadMapFromFile(filePath);
	InitializeMap(device, deviceContext, cbVertexShader);
}

Map::~Map()
{
}

void Map::Draw(const XMMATRIX& viewProjectionMatrix, Light* ambientLight, DirectionalLight* directionalLight, PointLight* pointLight)
{
	for (size_t floor = 0; floor < m_floor.size(); floor++)
	{
		m_floor[floor]->Draw(viewProjectionMatrix, ambientLight, directionalLight, pointLight);
	}

	for (size_t walls = 0; walls < m_walls.size(); walls++)
	{
		m_walls[walls]->Draw(viewProjectionMatrix, ambientLight, directionalLight, pointLight);
	}

	for (size_t rock = 0; rock < Rock::RockList.size(); rock++)
	{
		Rock::RockList[rock]->Draw(viewProjectionMatrix, ambientLight, directionalLight, pointLight);
	}

	for (size_t healthKit = 0; healthKit < HealthKit::HealthKitList.size(); healthKit++)
	{
		HealthKit::HealthKitList[healthKit]->Draw(viewProjectionMatrix, ambientLight, directionalLight, pointLight);
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
			case 'F':
			{
				CreateFloor(device, deviceContext, cbVertexShader, i, j);
				break;
			}
			case 'W':
			{
				CreateWall(device, deviceContext, cbVertexShader, i, j);
				break;
			}
			case 'V':
			{
				CreateFloor(device, deviceContext, cbVertexShader, i, j);
				CreateRock(device, deviceContext, cbVertexShader, i, j, false);
				break;
			}
			case 'H':
			{
				CreateFloor(device, deviceContext, cbVertexShader, i, j);
				CreateRock(device, deviceContext, cbVertexShader, i, j, true);
				break;
			}
			case 'P':
			{
				CreateFloor(device, deviceContext, cbVertexShader, i, j);
				m_playerStartPosition = XMFLOAT3(i, 2.0f, j);
				break;
			}
			case 'K':
			{
				CreateFloor(device, deviceContext, cbVertexShader, i, j);
				CreateHealthKit(device, deviceContext, cbVertexShader, i, j);
				break;
			}
			}
		}
	}
}

std::vector<GameObject*> Map::GetWalls()
{
	return m_walls;
}

XMFLOAT3 Map::GetPlayerStartPosition()
{
	return m_playerStartPosition;
}

void Map::CreateFloor(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column)
{
	GameObject* floor = new GameObject();
	floor->Initialise(device, deviceContext, "Assets/Models/cube.obj", "Assets/Textures/seamless_pavement.jpg", "ModelShader.cso", "PixelShader.cso", cbVertexShader);
	floor->SetPosition(row * 2, 0.0f, column * 1);
	m_floor.push_back(floor);
}

void Map::CreateWall(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column)
{
	GameObject* wall = new GameObject();
	wall->SetPosition(row * 2, 3.0f, column * 1);
	wall->SetScale(1.0f, 3.0f, 1.0f);
	wall->Initialise(device, deviceContext, "Assets/Models/cube.obj", "Assets/Textures/Wall1.png", "ModelShader.cso", "PixelShader.cso", cbVertexShader);
	m_walls.push_back(wall);
}

void Map::CreateRock(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column, bool isHorizontal)
{
	Rock* rock = new Rock(device, deviceContext, 0.01f, "Assets/Models/Sphere.obj", "Assets/Textures/Rock.png", "ModelShader.cso", "PixelShader.cso", cbVertexShader, this);
	rock->SetPosition(row * 2, 1.75f, column * 1);
	rock->SetScale(0.25f, 0.25f, 0.25f);
	rock->SetDirection(isHorizontal);
}

void Map::CreateHealthKit(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_Model>& cbVertexShader, int row, int column)
{
	HealthKit* healthKit = new HealthKit(device, deviceContext, "Assets/Models/bottle_01.obj", "Assets/Textures/bottle_01.png", "ModelShader.cso", "PixelShader.cso", cbVertexShader);
	healthKit->SetPosition(row * 2.0f, 1.0f, column * 0.85f);
	healthKit->SetScale(0.08f, 0.08f, 0.08f);
}

