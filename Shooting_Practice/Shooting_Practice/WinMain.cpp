/////////////////////
// INCLUDE
/////////////////////
#include "WinMain.h"
#include <cstdio>

#ifdef _DEBUG
#pragma comment(linker, "/⁄entry:WinMainCRTStartup /⁄subsystem:console")
#endif

////////////////////////////////////
// Main of Window.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	Engine *engine = NULL;
	bool rs;

	// initialize Window & Create Window.
	rs = InitWnd(hInstance, nCmdShow);
	if (!rs) {
		MessageBox(NULL, L"Failed to initialize Window.", L"Error", MB_OK);
		return 0;
	}

	engine = new Engine;
	if (!engine) 
		return 0;

	rs = engine->Init(hInstance, g_hWnd, g_width, g_height);
	if (!rs) {
		MessageBox(NULL, L"Failed to initialize Engine.", L"Error", MB_OK);
		return 0;
	}
	Initialized = true;

	RedirectIOToConsole(); // 디버깅용 콘솔창 

	FILE *p;
	freopen_s(&p, "CONOUT$", "wt", stdout); 
	engine->Run();

	engine->Release();
	::FreeConsole(); // 콘솔창을 제거한다.
	delete engine;

	return 0;
}

////////////////////////////////////
// Initialize Window Class and Create.
bool InitWnd(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wndEx = {};
	int xpos, ypos;

	g_width = 1024;
	g_height = 768;

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

	return true;
}

////////////////////////////////////
// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}