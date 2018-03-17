#include "Engine.h"

Engine::Engine()
{
	graphic = NULL;
	input = NULL;
	hWnd = 0;
}


Engine::~Engine()
{
}


void Engine::Init(HWND hWnd, int width, int height)
{
	this->hWnd = hWnd;

	this->screenW = width;
	this->screenW = height;

	graphic = new Graphic;
	if (!graphic)
		return;
	graphic->Init(hWnd, width, height);

	input = new Input;
	if (!input)
		return;

	CreateObject();

	return;
}

void Engine::Run()
{
	graphic->Frame(plane, backGround, hWnd);

	return;
}

void Engine::Release()
{

	return;
}

LRESULT Engine::InputProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return input->MsgHandle(hWnd, msg, wParam, lParam);
}

bool Engine::IsWin() {

	return true;
}

void Engine::CreateObject()
{
	// 0 ~ 800 ¿Œ∞‘¿” 801~1024 UI 
	// 1024 x 768
	//for (int i = 0; i < MAX_ENERMY; i++) {

	//for (int i = 0; i < MAX_BACKGROUND; i++) {
	Object temp_player;
	temp_player.InitObject(graphic->GetDevice(), 512.0f, 700.0f, 50.0f, 50.0f, 3.0f, screenW, screenH, "PLAYER", Player);
	plane.push_back(temp_player);

	for (int i = 1; i <= MAX_ENERMY; i++) {
		Object temp;
		temp.InitObject(graphic->GetDevice(), (0.0f + i) * 100.0f, 0.0f, 50.0f, 50.0f, 3.0f, screenW, screenH, "ENERMY", Enermy_1);
		plane.push_back(temp);
	}

	for (int i = 0; i < MAX_BACKGROUND; i++) {
		Object temp_back;
		temp_back.InitObject(graphic->GetDevice(), 0.0f, (0.0f + i) * -768.0f, 700.0f, 768.0f, 1.0f, screenW, screenH, "BACKGROUND", Background_Space);
		backGround.push_back(temp_back);
	}

	//}

	return;
}