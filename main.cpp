// NOTE: The ordering of these #includes are important
#include "Engine.h"

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR lpCmd, _In_ int nCmdShow)
{
	Engine engine;

	engine.Initliase(hInst, "Title", "MyWindowClass", 800, 600);

	while (engine.ProcessMessages())
	{
		engine.Update();
	}

	return 0;
}