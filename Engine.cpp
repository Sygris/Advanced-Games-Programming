#include "Engine.h"

bool Engine::Initliase(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	bool result;
	result = m_window.Initliase(this, hInstance, windowTitle, windowClass, width, height);

	m_keyboard = new Keyboard(hInstance, m_window.GetHandler());
	m_keyboard->InitialiseInput();

	return result;
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
