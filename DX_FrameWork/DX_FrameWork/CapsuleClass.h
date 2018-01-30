#pragma once
#include <Windows.h>
#include "RenderClass.h"
#include "InputClass.h"

class CapsuleClass {
public:
	CapsuleClass();
	~CapsuleClass();

	HRESULT InitEngine(HWND, int, int, bool);
	void Release();
	void Frame();

private:
	RenderClass *render;
	InputClass *input;
};