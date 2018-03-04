/////////////////////
// INCLUDE
/////////////////////
#include "WinMain.h"

////////////////////////////////////
// Main of Window.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	// initialize Window & Create Window.
	InitWnd(hInstance, nCmdShow);

	engine = new Engine;
	if (!engine)
		return 0;
	engine->Init(g_hWnd, g_width, g_height);
	Initialized = true;

	MSG Message = { 0 };
	// Game Loop
	while (WM_QUIT != Message.message)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			engine->Run();
		}
	}

	engine->Release();

	return (int)Message.wParam;
}

////////////////////////////////////
// Initialize Window Class and Create.
int InitWnd(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wndEx;
	int xpos, ypos;

	g_width = 1280;
	g_height = 800;

	xpos = (GetSystemMetrics(SM_CXSCREEN) - g_width) / 2;
	ypos = (GetSystemMetrics(SM_CYSCREEN) - g_height) / 2;

	wndEx.cbSize = sizeof(WNDCLASSEX);
	wndEx.cbClsExtra = 0;
	wndEx.cbWndExtra = 0;
	wndEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndEx.hIconSm = NULL;
	wndEx.hInstance = hInstance;
	wndEx.lpfnWndProc = WndProc;
	wndEx.lpszClassName = g_title;
	wndEx.lpszMenuName = NULL;
	wndEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClassEx(&wndEx);

	g_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		g_title,
		g_title,
		WS_OVERLAPPED,// | WS_CAPTION,
		xpos,
		ypos,
		g_width,
		g_height,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	if (!g_hWnd)
		return false;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

////////////////////////////////////
// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if(Initialized)
		return engine->InputProcess(hWnd, iMessage, wParam, lParam);
	else
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
}