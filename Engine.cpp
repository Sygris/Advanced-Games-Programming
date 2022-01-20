#include "Engine.h"
#include "Entities/HealthKit.h"

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

	m_graphics.m_player->SetupInput(m_keyboard, m_mouse);

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

	m_graphics.m_player->Update(deltaTime);

	for (size_t rock = 0; rock < Rock::RockList.size(); rock++)
	{
		Rock::RockList[rock]->Update(deltaTime);
	}

	for (size_t healthKit = 0; healthKit < HealthKit::HealthKitList.size(); healthKit++)
	{
		HealthKit::HealthKitList[healthKit]->Update(deltaTime);
	}

	std::string lifesText = "Lifes left " + std::to_string(m_graphics.m_player->GetLifes());
	m_graphics.m_text->AddText(lifesText, -0.99f, 0.99f, 0.045f);

	m_graphics.m_pointLight->SetPosition(m_graphics.m_player->GetCamera()->GetPositionVector());
}

void Engine::RenderFrame()
{
	m_graphics.RenderFrame();
}
