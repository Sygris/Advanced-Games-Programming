#pragma once
// Had to add this to fix unresolved external symbol 
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION 0x0800 // Sets the DirectInput version
#include <d3d11.h>
#include <dinput.h>

class Keyboard
{
public:
	Keyboard(HINSTANCE hInstance, HWND hwnd);
	~Keyboard();

	HRESULT InitialiseInput();
	void ReadInputStates();
	bool IsKeyPreesed(unsigned char keycode);
private:
	HINSTANCE m_hInst;
	HWND m_hwnd;
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboardDevice;
	unsigned char m_keyboardStates[256];
};

