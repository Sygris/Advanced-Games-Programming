#include "Rock.h"
#include "../Map.h"
#include "../Physics/Colliders.h"

std::vector<Rock*> Rock::RockList;

Rock::Rock(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float speed, const char* objPath, const char* texturePath, const char* vertexShader, const char* pixelShader,
	ConstantBuffer<CB_VS_Model>& cbVertexShader, Map* map)
	: m_speed{ speed }, m_map{ map }
{
	Initialise(device, deviceContext, objPath, texturePath, vertexShader, pixelShader, cbVertexShader);
	RockList.push_back(this);
}

Rock::~Rock()
{
	if (m_map != nullptr)
	{
		delete m_map;
		m_map = nullptr;
	}

	for (std::vector<Rock*>::iterator iter = RockList.begin(); iter != RockList.end(); ++iter)
	{
		if (*iter == this)
		{
			RockList.erase(iter);
			break;
		}
	}
}

void Rock::Update(float deltaTime)
{
	XMVECTOR newPosition;

	if (isHorizontalMovement)
		newPosition = m_positionVector + XMVECTOR{ m_speed * deltaTime, 0.0f, 0.0f, 1.0f };
	else
		newPosition = m_positionVector + XMVECTOR{ 0.0f, 0.0f, m_speed * deltaTime, 1.0f };

	for (int i = 0; i < m_map->GetWalls().size(); i++)
	{
		if (BoxToSphereCollision(this, m_map->GetWalls()[i], newPosition))
		{
			m_speed = -m_speed;
			return;
		}
	}

	if (isHorizontalMovement)
		AdjustPosition(XMVECTOR{ m_speed * deltaTime, 0.0f, 0.0f, 1.0f });
	else
		AdjustPosition(XMVECTOR{ 0.0f, 0.0f, m_speed * deltaTime, 1.0f });

	AdjustRotation(m_rotationSpeed);
}

void Rock::SetDirection(bool horizontal)
{
	isHorizontalMovement = horizontal;
}
