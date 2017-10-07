//---------------------------------------------------------------------------------
// File: WinMain.h
//-------------------------------------------------------------------------------------

#pragma once

//-------------
// INCLUDE
//-------------
#include <Windows.h>

//---------------
// STRUCTURES
//---------------
typedef struct {
	int H; // Height of Screen.
	int W; // Width of Screen.
}SCREEN_SIZE;

//---------------
// GLOBAL
//---------------
static SCREEN_SIZE	screen;
static HINSTANCE	g_hInst = NULL;
static const LPCTSTR		lpszClass = TEXT("DirectX Frame Work"); // Name of Window Title bar.
static HWND			g_hwnd = NULL;								  // Window Handle. 

// Initialize WindowStyle Class
int APIENTRY InitWindow(HINSTANCE, int, int, int);

// Call InitWindow and 
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

// Proccesing Window Message
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
