#include "WindowContainer.h"
#include <DxErr.h>

bool Window::Initliase(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    m_hInstance = hInstance;
    m_windowTitle = windowTitle;
    m_windowClassName = windowClass;
    m_width = width;
    m_height = height;

    RegisterWindowClass();

	RECT rc = { 0, 0, width, height };

	AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

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
		pWindowContainer
	);

	CenterWindow();

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

void Window::CenterWindow()
{
	RECT rc;
	GetWindowRect(m_handle, &rc);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	SetWindowPos(m_handle, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

Window::~Window()
{
	if (m_handle != NULL)
	{
		UnregisterClass(m_windowClassName.c_str(), m_hInstance);
		DestroyWindow(m_handle);
	}
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE))
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

#pragma region Message Handler Setup and Redirect
LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
	{
		// Retrives ptr to Window class
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// Redirect message to windwo class handler
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		// Extract ptr to WindowContainer class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

		// Sanity check
		if (pWindow == nullptr)
		{
			DXTRACE_MSG(TEXT("Critical Error: Pointer to window container is null during WM_NCCREATE"));
			exit(-1);
		}

		// Set WinAPI-managed user data to store ptr to WindowContainer class
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		// Set message proc to normal handler now that setup is finished
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));
		// Redirect message to WindowContainer class handler
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
#pragma endregion

void Window::RegisterWindowClass()
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = HandleMessageSetup;
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