#include "WinMain.h"
#include "CapsuleClass.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CapsuleClass *capsuleClass = NULL;

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	InitWindow(hInstance, nCmdShow, width, height);
	capsuleClass->InitEngine(g_hwnd, width, height, WINDOW_MODE);

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
			capsuleClass->Frame();
		}
	}

	capsuleClass->Release();

	return (int)Message.wParam;
}

int APIENTRY InitWindow(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	WNDCLASSEX wndEx;
	int Xpos, Ypos;
	DEVMODE scrSet;

	if (WINDOW_MODE)
	{
		width = 1000;
		height = 800;

		Xpos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		Ypos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}
	else
	{
		memset(&scrSet, 0, sizeof(scrSet));

		scrSet.dmSize = sizeof(scrSet);
		scrSet.dmPelsWidth = (unsigned long)width;
		scrSet.dmPelsHeight = (unsigned long)height;
		scrSet.dmBitsPerPel = 32; //32bit = R8 G8 B8 A8 
		scrSet.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&scrSet, CDS_FULLSCREEN);

		Xpos = Ypos = 0;
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
	wndEx.lpszClassName = lpszClass;
	wndEx.lpszMenuName = NULL;
	wndEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClassEx(&wndEx);

	g_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		lpszClass,
		lpszClass,
		WS_BORDER,
		Xpos,  // Start posX
		Ypos,  // Start posY
		width,  // Width
		height,  // Height
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	if (!g_hwnd)
		return false;

	ShowWindow(g_hwnd, nCmdShow);

	return S_OK;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, iMessage, wParam, lParam);
	}
	return 0;
}