#pragma once
//////////////////////
// INCLUDE 
/////////////////////
#include <Windows.h>
#include "Engine.h"
#include "Console.h"

/////////////////////
// CONSTANT
////////////////////


////////////////////////
// GLOBAL 
////////////////////////
HINSTANCE		g_hInst = NULL;
static HWND			g_hWnd = NULL;
LPCTSTR			g_title = TEXT("Shooting Game Practice");

int g_width;
int g_height;
bool Initialized = false;

////////////////////////
// FUNCTION DECLARE
////////////////////////

////////////////////////////////////
// Main of Window.
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

////////////////////////////////////
// Initialize Window Class and Create.
bool InitWnd(HINSTANCE, int);

////////////////////////////////////
// Window Procedure
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);