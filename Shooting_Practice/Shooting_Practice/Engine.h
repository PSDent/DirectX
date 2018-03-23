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
const int MAX_ENEMY = 19;
const int MAX_BACKGROUND = 2;
const int MAX_PROJECTION = 30;
const float TERM = 80.0f;
const int SPAWN_POINT = 9;

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
	void Spawn();
	void CreateObject();
	void ApplyInput();

private:
	int screenW, screenH;
	bool done;
	bool *spawnPoint;
	HINSTANCE hInstance;

	Graphic *graphic;
	Input *input;
	GameManager *gameMgr;

	vector<Object> backGround;
	// obj[0] is PLAYER. 
	// Another objects are enemy.
	vector<Object> plane; 
	HWND hWnd;

};

