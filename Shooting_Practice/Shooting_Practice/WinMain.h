#pragma once
//////////////////////
// INCLUDE 
/////////////////////
#include <Windows.h>
#include "Engine.h"

/////////////////////
// CONSTANT
////////////////////


////////////////////////
// GLOBAL 
////////////////////////
HINSTANCE		g_hInst = NULL;
HWND			g_hWnd = NULL;
LPCTSTR			g_title = TEXT("Shooting Game Practice");
Engine			*engine = NULL;

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
int InitWnd(HINSTANCE, int);

////////////////////////////////////
// Window Procedure
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);