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
	graphic->Frame(plane);

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
	//for (int i = 0; i < MAX_ENERMY; i++) {
	Object temp;
	temp.InitObject(graphic->GetDevice(), 100, 100, 50, 50, L"C:\\Users\\user\\Downloads\\SpaceShooterRedux\\PNG\\playerShip1_blue.png");
	plane.push_back(temp);
	//}

	return;
}