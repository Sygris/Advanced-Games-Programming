#pragma once
#include <chrono>

class Timer
{
public:
	Timer();

	double GetMilisecondsElapsed();

	bool Start();
	bool Stop();
	void Restart();
private:
	bool m_isRunning = false;

	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_stop;
};

