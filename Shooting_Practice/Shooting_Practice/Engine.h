#pragma once

#include <Windows.h>
#include <Vector>
#include "Object.h"
#include "Graphic.h"
#include "Input.h"
#include "GameManager.h"

///////////////////
// System Value
const int MAX_ENERMY = 5;

class Engine
{
public:
	Engine();
	~Engine();

	bool IsWin();
	void Init(HWND, int, int);
	void Run();
	void Release();
	LRESULT InputProcess(HWND, UINT, WPARAM, LPARAM);

private:
	void CreateObject();

private:
	Graphic *graphic;
	Input *input;

	vector<Object> backGround;
	// obj[0] is Player. 
	// Another objects are Enermy.
	vector<Object> plane; 
	HWND hWnd;

};

