#pragma once
#include <Windows.h>
#include "D3DClass.h"

//===============
// Constant
//===============
const bool vsync = true;
const float screenNear = 0.0f;
const float screenFar = 1000.0f;


class RenderClass {
public:
	void Init(HWND, int, int, float, float, bool, bool); 
	void Frame();

	RenderClass();
	~RenderClass();

private:
	D3DClass *d3d;
};