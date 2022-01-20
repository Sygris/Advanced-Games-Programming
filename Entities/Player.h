#pragma once
#include "../Graphics/Camera.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../Map.h"

class Player
{
public:
	Player(Map* map, Camera* camera);
	~Player();

	void Update(float deltaTime);

	void SetLifes(int lifes);
	int GetLifes();
	
	Camera* GetCamera();

	void SetupInput(Keyboard* keyboard, Mouse* mouse);
private:
	void Forward(float deltaTime);
	void Backward(float deltaTime);
	void Left(float deltaTime);
	void Right(float deltaTime);

	void CheckCollisions();

	void Hit();
	void Respawn();

	Keyboard* m_keyboard;
	Mouse* m_mouse;
	Camera* m_camera;
	Map* m_map;

	int m_lifes;
	float m_speed;
	float m_rotationSpeed;
};

