#include "Engine.h"

bool Engine::Initliase(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	m_timer.Start();

	if(!m_window.Initliase(this, hInstance, windowTitle, windowClass, width, height)) 
		return false;

	if (!m_graphics.Initialise(m_window.GetHandler(), width, height))
		return false;

	m_keyboard = new Keyboard(hInstance, m_window.GetHandler());
	m_keyboard->InitialiseInput();

	return true;
}

bool Engine::ProcessMessages()
{
	return m_window.ProcessMessages();
}

void Engine::Update()
{
	float deltaTime = m_timer.GetMilisecondsElapsed();
	m_timer.Restart();

	m_keyboard->ReadInputStates();

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
}

void Engine::RenderFrame()
{
	m_graphics.RenderFrame();
}
