//---------------------------------------------------------------------------------
// File: WinMain.h
//-------------------------------------------------------------------------------------

#pragma once

//-------------
// INCLUDE
//-------------
#include <Windows.h>

//---------------
// GLOBAL
//---------------
HINSTANCE	g_hInst = NULL;
LPCTSTR		lpszClass = TEXT("DirectX Frame Work"); // Name of Window Title bar.
HWND		g_hwnd = NULL;								  // Window Handle. 

// Initialize WindowStyle Class
int APIENTRY InitWindow(HINSTANCE, int, int, int);

// Call InitWindow and 
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

// Proccesing Window Message
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);