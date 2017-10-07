//---------------------------------------------------------------------------------
// File: RenderClass.h
//-------------------------------------------------------------------------------------
#pragma once
#include "D3DClass.h"

#define WINDOW_MODE true
#define VSYNC       true
#define SCREEN_FAR  1000.0f
#define SCREEN_NEAR 0.1f

class RenderClass {
public:
	D3DClass *d3dclass;

public:
	RenderClass();
	~RenderClass(); 

	void Release();
	bool Setup(HWND hwnd, int, int);
	void Update();
};