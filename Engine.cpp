#include "Engine.h"

bool Engine::Initliase(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	m_timer.Start();

	if (!m_window.Initliase(this, hInstance, windowTitle, windowClass, width, height))
		return false;

	if (!m_graphics.Initialise(m_window.GetHandler(), width, height))
		return false;

	m_keyboard = new Keyboard(hInstance, m_window.GetHandler());
	m_keyboard->Initialise();

	m_mouse = new Mouse(hInstance, m_window.GetHandler());
	m_mouse->Initialise();

	return true;
}

bool Engine::ProcessMessages()
{
	return m_window.ProcessMessages();
}

void Engine::Update()
{
	float deltaTime = static_cast<float>(m_timer.GetMilisecondsElapsed());
	m_timer.Restart();

	m_keyboard->ReadInputStates();
	m_mouse->ReadInputStates();

	m_graphics.m_sphere.AdjustRotation(0.0f, 0.001f * deltaTime, 0.0f);

	if (m_mouse->ButtonDown(0))
	{
		m_graphics.m_camera.AdjustRotation((float)m_mouse->GetMousePosY() * 0.005f, (float)m_mouse->GetMousePosX() * 0.005f, 0);
	}

	if (m_keyboard->IsKeyPreesed(DIK_W))
	{
		m_graphics.m_camera.AdjustPosition(m_graphics.m_camera.GetForwardVector() * 0.01f * deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_A))
	{
		m_graphics.m_camera.AdjustPosition(m_graphics.m_camera.GetLeftVector() * 0.01f * deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_S))
	{
		m_graphics.m_camera.AdjustPosition(m_graphics.m_camera.GetBackwardVector() * 0.01f * deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_D))
	{
		m_graphics.m_camera.AdjustPosition(m_graphics.m_camera.GetRightVector() * 0.01f * deltaTime);
	}

	if (m_keyboard->IsKeyPreesed(DIK_SPACE))
	{
		m_graphics.m_camera.AdjustPosition(0.0f, 0.01f * deltaTime, 0.0f);
	}
	if (m_keyboard->IsKeyPreesed(DIK_Z))
	{
		m_graphics.m_camera.AdjustPosition(0.0f, -0.01f * deltaTime, 0.0f);
	}

	m_graphics.m_text->AddText("Move WASD", -0.99f, 0.99f, 0.045f);
	m_graphics.m_text->AddText("Rotate Hold Left Click on the mouse", -0.99f, 0.89f, 0.045f);
	m_graphics.m_text->AddText("UP Space Bar", -0.99f, 0.79f, 0.045f);

	if (m_keyboard->IsKeyPreesed(DIK_RIGHTARROW))
	{
		m_graphics.m_pointySphere.AdjustPosition(m_graphics.m_pointySphere.GetRightVector() * 0.01f * deltaTime);

		if (SphereToSphereCollision(m_graphics.m_pointySphere, m_graphics.m_sphere))
		{
			m_graphics.m_pointySphere.AdjustPosition(m_graphics.m_pointySphere.GetLeftVector() * 0.01f * deltaTime);
		}
	}

	if (m_keyboard->IsKeyPreesed(DIK_LEFTARROW))
	{
		m_graphics.m_pointySphere.AdjustPosition(m_graphics.m_pointySphere.GetLeftVector() * 0.01f * deltaTime);

		if (SphereToSphereCollision(m_graphics.m_pointySphere, m_graphics.m_sphere))
		{
			m_graphics.m_pointySphere.AdjustPosition(m_graphics.m_pointySphere.GetRightVector() * 0.01f * deltaTime);
		}
	}
}

void Engine::RenderFrame()
{
	m_graphics.RenderFrame();
}
