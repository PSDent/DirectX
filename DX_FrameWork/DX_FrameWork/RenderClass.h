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
	HRESULT Init(HWND, int, int, bool);
	void Frame();
	void Release();

	RenderClass();
	~RenderClass();

private:
	D3DClass *d3d;
};