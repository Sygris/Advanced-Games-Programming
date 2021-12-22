#include "Engine.h"

bool Engine::Initliase(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
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
	m_keyboard->ReadInputStates();

	if (m_keyboard->IsKeyPreesed(DIK_A))
	{
	}
}

void Engine::RenderFrame()
{
	m_graphics.RenderFrame();
}
