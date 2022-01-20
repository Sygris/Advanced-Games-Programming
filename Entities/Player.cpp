#include "Player.h"
#include "Rock.h"
#include "HealthKit.h"

Player::Player(Map* map, Camera* camera)
	: m_map{ map }, m_camera{ camera }
{
	m_lifes = 3;
	m_speed = 0.005f;
	m_rotationSpeed = 0.005f;

	m_camera->GetCollider()->m_colliderCentre = m_camera->GetPositionFloat3();
	m_camera->GetCollider()->m_radius = 0.3f;

	m_camera->SetPosition(m_map->GetPlayerStartPosition());
}

Player::~Player()
{
	if (m_camera != nullptr)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	if (m_map != nullptr)
	{
		delete m_map;
		m_map = nullptr;
	}

	if (m_keyboard != nullptr)
	{
		delete m_keyboard;
		m_keyboard = nullptr;
	}

	if (m_mouse != nullptr)
	{
		delete m_mouse;
		m_mouse = nullptr;
	}
}

void Player::Update(float deltaTime)
{
	m_keyboard->ReadInputStates();
	m_mouse->ReadInputStates();

	m_camera->AdjustRotation(0, (float)m_mouse->GetMousePosX() * m_rotationSpeed, 0);

	if (m_keyboard->IsKeyPreesed(DIK_W))
	{
		Forward(deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_A))
	{
		Left(deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_S))
	{
		Backward(deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_D))
	{
		Right(deltaTime);
	}

	CheckCollisions();
}

void Player::SetLifes(int lifes)
{
	m_lifes += lifes;
}

int Player::GetLifes()
{
	return m_lifes;
}

Camera* Player::GetCamera()
{
	return m_camera;
}

void Player::SetupInput(Keyboard* keyboard, Mouse* mouse)
{
	m_keyboard = keyboard;
	m_mouse = mouse;
}

void Player::Forward(float deltaTime)
{
	XMVECTOR newPosition = m_camera->GetPositionVector() + m_camera->GetForwardVector() * deltaTime * m_speed;

	for (int i = 0; i < m_map->GetWalls().size(); i++)
	{
		if (BoxToSphereCollision(m_camera->GetCollider(), m_map->GetWalls()[i], newPosition))
			return;
	}

	// Move
	m_camera->AdjustPosition(m_camera->GetForwardVector() * deltaTime * m_speed);
}

void Player::Backward(float deltaTime)
{
	XMVECTOR newPosition = m_camera->GetPositionVector() + m_camera->GetBackwardVector() * deltaTime * m_speed;

	for (int i = 0; i < m_map->GetWalls().size(); i++)
	{
		if (BoxToSphereCollision(m_camera->GetCollider(), m_map->GetWalls()[i], newPosition))
			return;
	}

	// Move
	m_camera->AdjustPosition(m_camera->GetBackwardVector() * deltaTime * m_speed);
}

void Player::Left(float deltaTime)
{
	XMVECTOR newPosition = m_camera->GetPositionVector() + m_camera->GetLeftVector() * deltaTime * m_speed;

	for (int i = 0; i < m_map->GetWalls().size(); i++)
	{
		if (BoxToSphereCollision(m_camera->GetCollider(), m_map->GetWalls()[i], newPosition))
			return;
	}

	// Move
	m_camera->AdjustPosition(m_camera->GetLeftVector() * deltaTime * m_speed);
}

void Player::Right(float deltaTime)
{
	XMVECTOR newPosition = m_camera->GetPositionVector() + m_camera->GetRightVector() * deltaTime * m_speed;

	for (int i = 0; i < m_map->GetWalls().size(); i++)
	{
		if (BoxToSphereCollision(m_camera->GetCollider(), m_map->GetWalls()[i], newPosition))
			return;
	}

	// Move
	m_camera->AdjustPosition(m_camera->GetRightVector() * deltaTime * m_speed);
}

void Player::CheckCollisions()
{
	for (int i = 0; i < Rock::RockList.size(); i++)
	{
		if (CameraToSphereCollision(m_camera, Rock::RockList[i]))
		{
			Hit();
			return;
		}
	}

	for (int j = 0; j < HealthKit::HealthKitList.size(); j++)
	{
		if (CameraToSphereCollision(m_camera, HealthKit::HealthKitList[j]))
		{
			// FUTURE: Add a better way to destroy the HealthKit
			SetLifes(1);
			HealthKit::HealthKitList[j]->~HealthKit();

		}
	}
}

void Player::Hit()
{
	m_lifes--;

	if (m_lifes < 1)
	{
		// FUTURE: Add a proper way to finish and close the game
		exit(0);
	}
	else
	{
		Respawn();
	}
}

void Player::Respawn()
{
	m_camera->SetPosition(m_map->GetPlayerStartPosition());
}
