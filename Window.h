#pragma once
#include <Windows.h>
#include <string>

class WindowContainer;

class Window
{
public:
	bool Initliase(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
	bool ProcessMessages();

	~Window();

	HWND GetHandler() { return m_handle; }
private:
	void RegisterWindowClass();
	
	HWND m_handle = nullptr; // Handle to this window
	HINSTANCE m_hInstance = nullptr; // Handle to the application instacnce

	std::string m_windowTitle = "";
	std::string m_windowClassName = "";

	int m_width = 0;
	int m_height = 0;
};