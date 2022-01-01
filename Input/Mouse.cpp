#include "Mouse.h"

Mouse::Mouse(HINSTANCE hInstance, HWND hwnd)
	: m_hInst{ hInstance }, m_hwnd{ hwnd }
{
}

HRESULT Mouse::Initialise()
{
    HRESULT hr;

    hr = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
    if (FAILED(hr)) return hr;

    hr = m_directInput->CreateDevice(GUID_SysMouse, m_mouseDevice.GetAddressOf(), 0);
    if (FAILED(hr)) return hr;

    hr = m_mouseDevice->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr)) return hr;

    hr = m_mouseDevice->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) return hr;

    hr = m_mouseDevice->Acquire();
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Mouse::ReadInputStates()
{
    HRESULT hr = m_mouseDevice->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);
    if (FAILED(hr))
    {
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
        {
            m_mouseDevice->Acquire();
        }
    }

    m_mousePosX = m_mouseState.lX;
    m_mousePosY = m_mouseState.lY;
    m_mouseWheel = m_mouseState.lZ;
}

// Left - 0, Right - 1, Middle - 2
bool Mouse::ButtonDown(int button) const
{
    if (m_mouseState.rgbButtons[button] & 0x80)
        return true;

    return false;
}

const int Mouse::GetMousePosX() const
{
    return m_mousePosX;
}

const int Mouse::GetMousePosY() const
{
    return m_mousePosY;
}

const int Mouse::GetMouseWheel() const
{
    return m_mouseWheel;
}
