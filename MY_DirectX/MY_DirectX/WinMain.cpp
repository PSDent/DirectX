//---------------------------------------------------------------------------------
// File: WinMain.cpp
//
// This Source is for Initialize Window and Show the Window.
//
// Copyright PSDent. All rights reserved.
//-------------------------------------------------------------------------------------

#include "WinMain.h"
#include "RenderClass.h"

//----------------------------------------------------------
// Window Main Function
// Call the InitWindow()
//----------------------------------------------------------

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	RenderClass *renderClass = new RenderClass;

	screen.H = GetSystemMetrics(SM_CYSCREEN);
	screen.W = GetSystemMetrics(SM_CXSCREEN);

	// Initialize Window to show 
	if(InitWindow(hInstance, nCmdShow, screen.W, screen.H) != S_OK)
		return false;

	// Initialize Direct3D 
	if(!renderClass->Setup(g_hwnd, screen.W, screen.H)) // There are many problem. you must fix it .
		return false;

	MSG Message = { 0 };

	while (WM_QUIT != Message.message)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			renderClass->Update();
		}
	}

	renderClass->Release();

	return (int)Message.wParam;
}

// Initialize Window to show Window 
int APIENTRY InitWindow(HINSTANCE hInstance, int nCmdShow, int width, int height)  
{
	WNDCLASSEX wndEx;

	g_hInst = hInstance;

	wndEx.cbSize = sizeof(WNDCLASSEX);
	wndEx.cbClsExtra = 0;
	wndEx.cbWndExtra = 0;
	wndEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndEx.hIconSm = NULL;
	wndEx.hInstance = hInstance;
	wndEx.lpfnWndProc = WndProc;
	wndEx.lpszClassName = lpszClass;
	wndEx.lpszMenuName = NULL;
	wndEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClassEx(&wndEx);

	g_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		lpszClass, 
		lpszClass,
		 WS_BORDER | WS_SYSMENU,
		screen.W / 4,  // Start posX
		screen.H / 6,  // Start posY
		screen.W - screen.W / 2,  // Width
		screen.H - screen.H / 3,  // Height
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	if (!g_hwnd)
		return false;

	ShowWindow(g_hwnd, nCmdShow);

	return S_OK;
}

//--------------------------------------------
// Message Proccesing procedure
//--------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) 
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, iMessage, wParam, lParam);  
	}
	return 0;
}

