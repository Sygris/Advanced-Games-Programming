// NOTE: The ordering of these #includes are important
#include "Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR lpCmd, _In_ int nCmdShow)
{
	Window window;

	window.Initliase(hInst, "Title", "MyWindowClass", 800, 600);

	while (window.ProcessMessages())
	{

	}

	return 0;
}