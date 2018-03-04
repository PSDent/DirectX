#pragma once

#include <Windows.h>
#include "Graphic.h"
#include "Input.h"

class Engine
{
public:
	Engine();

	~Engine();

	void Init(HWND, int, int);
	void Run();
	void Release();
	LRESULT InputProcess(HWND, UINT, WPARAM, LPARAM);

private:
	Graphic *graphic;
	Input *input;
	HWND hWnd;

};

