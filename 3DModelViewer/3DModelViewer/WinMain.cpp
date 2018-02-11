/////////////////////
// INCLUDE
/////////////////////
#include "WinMain.h"
#include "Engine.h"
#include "ModelParser.h"

////////////////////////////////////
// Main of Window.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	// initialize Window & Create Window.
	InitWnd(hInstance, nCmdShow, width, height);

	// Load Model File & Parsing obj to my format.
	/*
	ModelParser *parser = NULL;
	parser = new ModelParser;
	parser->Parsing(g_hWnd);
	*/

	// Initialize DirectX 3D.
	Engine *engine = NULL;
	engine = new Engine;
	engine->InitEngine(g_hWnd, width, height, "C:\\Users\\user\\Desktop\\Model(obj)\\model.txt");

	MSG Message = { 0 };
	while (WM_QUIT != Message.message) 
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			engine->Frame();
		}
	}

	engine->Release();

	return (int)Message.wParam;
}

////////////////////////////////////
// Initialize Window Class and Create.
int InitWnd(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	WNDCLASSEX wndEx;
	int xpos, ypos;
	DEVMODE devSet;

	if (windowMode)
	{
		width = 1280;
		height = 800;

		xpos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		ypos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}
	else
	{
		memset(&devSet, 0, sizeof(devSet));

		devSet.dmSize = sizeof(devSet);
		devSet.dmPelsWidth = (unsigned long)width;
		devSet.dmPelsHeight = (unsigned long)height;
		devSet.dmBitsPerPel = 32;
		devSet.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devSet, CDS_FULLSCREEN);

		xpos = ypos = 0;
	}

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
		WS_BORDER,
		xpos,
		ypos,
		width,
		height,
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
	switch (iMessage) {
	case WM_KEYDOWN :
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return 0;
}