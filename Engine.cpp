#include "Engine.h"

bool Engine::Initliase(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	return m_window.Initliase(this, hInstance, windowTitle, windowClass, width, height);
}

bool Engine::ProcessMessages()
{
	return m_window.ProcessMessages();
}

void Engine::Update()
{
}
