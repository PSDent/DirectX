#pragma once

#include <Windows.h>
#include <Vector>
#include "Object.h"
#include "Graphic.h"
#include "Input.h"
#include "GameManager.h"
#include "ResourcePath.h"

///////////////////
// System Value
const float SPEED = 12.0f;
const int MAX_ENERMY = 5;
const int MAX_BACKGROUND = 2;

class Engine
{
public:
	Engine();
	~Engine();

	bool IsWin();
	bool Init(HINSTANCE, HWND, int, int);
	void Run();
	bool Frame();
	void Release();

private:
	void CreateObject();
	void ApplyInput();

private:
	int screenW, screenH;
	bool done;
	HINSTANCE hInstance;

	Graphic *graphic;
	Input *input;

	vector<Object> backGround;
	// obj[0] is Player. 
	// Another objects are Enermy.
	vector<Object> plane; 
	HWND hWnd;

};

