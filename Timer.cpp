#include "Timer.h"

Timer::Timer()
{
	m_start = std::chrono::high_resolution_clock::now();
	m_stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed()
{
	if (m_isRunning)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(m_stop - m_start);
		return elapsed.count();
	}
}

bool Timer::Start()
{
	if (m_isRunning)
		return false;

	m_start = std::chrono::high_resolution_clock::now();
	m_isRunning = true;
	return true;
}

bool Timer::Stop()
{
	if (!m_isRunning)
		return false;

	m_stop = std::chrono::high_resolution_clock::now();
	m_isRunning = false;
	return true;
}

void Timer::Restart()
{
	m_isRunning = true;
	m_start = std::chrono::high_resolution_clock::now();
}
