#pragma once

#include <Windows.h>
#include <Vector>
#include "Object.h"
#include "Graphic.h"
#include "Input.h"
#include "GameManager.h"

///////////////////
// System Value
const float SPEED = 12.0f;
const int MAX_ENERMY = 5;
const int MAX_BACKGROUND = 2;

//////////////////
// Texture Path
static const WCHAR *Enermy_1 = L"..\\Shooting_Practice\\Resource\\PNG\\Enemies\\enemyBlack1.png";
static const WCHAR *Enermy_2 = L"..\\Shooting_Practice\\Resource\\PNG\\Enemies\\enemyBlack2.png";
static const WCHAR *Enermy_3 = L"..\\Shooting_Practice\\Resource\\PNG\\Enemies\\enemyBlack3.png";
static const WCHAR *Player  =  L"..\\Shooting_Practice\\Resource\\PNG\\Player_1.png";
static const WCHAR *Background_Space = L"..\\Shooting_Practice\\Resource\\Backgrounds\\bg.png";

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
	LRESULT InputProcess(HWND, UINT, WPARAM, LPARAM);

private:
	void CreateObject();

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

