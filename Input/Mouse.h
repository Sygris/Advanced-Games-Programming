#pragma once
// Had to add this to fix unresolved external symbol 
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION 0x0800 // Sets the DirectInput version
#include <d3d11.h>
#include <dinput.h>

#include <wrl/client.h>

class Mouse
{
public:
	Mouse(HINSTANCE hInstance, HWND hwnd);

	HRESULT Initialise();
	void ReadInputStates();

	bool ButtonDown(int button) const;
	const int GetMousePosX() const;
	const int GetMousePosY() const;
	const int GetMouseWheel() const;
private:
	HINSTANCE m_hInst;
	HWND m_hwnd;

	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_mouseDevice;
	DIMOUSESTATE m_mouseState = {};
	int m_mousePosX  = 0;
	int m_mousePosY  = 0;
	int m_mouseWheel = 0;;
};


