#pragma once
#include "Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Graphics/Graphics.h"

/// <summary>
/// Class that contains our Window and other classes relevant to the window, e.g: Mouse, Keyboard, WndProc
/// </summary>
class WindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	Window m_window;
	Keyboard* m_keyboard;
	Mouse* m_mouse;
	Graphics m_graphics;
private:

};

