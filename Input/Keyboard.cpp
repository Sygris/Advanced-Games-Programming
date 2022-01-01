#include "Keyboard.h"

Keyboard::Keyboard(HINSTANCE hInstance, HWND hwnd)
    : m_hInst{hInstance}, m_hwnd{hwnd}
{
}

HRESULT Keyboard::Initialise()
{
    HRESULT hr;
    ZeroMemory(m_keyboardStates, sizeof(m_keyboardStates));

    hr = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
    if (FAILED(hr)) return hr;

    hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, NULL);
    if (FAILED(hr)) return hr;

    hr = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr)) return hr;

    hr = m_keyboardDevice->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) return hr;

    hr = m_keyboardDevice->Acquire();
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Keyboard::ReadInputStates()
{
    HRESULT hr;
    hr = m_keyboardDevice->GetDeviceState(sizeof(m_keyboardStates), (LPVOID)&m_keyboardStates);

    if (FAILED(hr))
    {
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
        {
            m_keyboardDevice->Acquire();
        }
    }
}

bool Keyboard::IsKeyPreesed(unsigned char DIK_Keycode)
{
    return m_keyboardStates[DIK_Keycode] & 0x80;
}
