#include "Window.h"
#include <DxErr.h>

bool Window::Initliase(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    m_hInstance = hInstance;
    m_windowTitle = windowTitle;
    m_windowClassName = windowClass;
    m_width = width;
    m_height = height;

    RegisterWindowClass();

	RECT rc = { 0, 0, m_width, m_height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_handle = CreateWindowEx(
		0,
		m_windowClassName.c_str(),
		m_windowTitle.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (m_handle == NULL)
	{
		DXTRACE_MSG(TEXT("Failed to Create a window!"));
		return false;
	}

	ShowWindow(m_handle, SW_SHOW);
	SetForegroundWindow(m_handle);
	SetFocus(m_handle);

    return true;
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Checks if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(m_handle))
		{
			m_handle = nullptr;
			UnregisterClass(m_windowClassName.c_str(), m_hInstance);
			return false;
		}
	}

    return true;
}

Window::~Window()
{
	if (m_handle != NULL)
	{
		UnregisterClass(m_windowClassName.c_str(), m_hInstance);
		DestroyWindow(m_handle);
	}
}

void Window::RegisterWindowClass()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_windowClassName.c_str();

	RegisterClassExA(&wcex);
}