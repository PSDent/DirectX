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

	return;
}

void Engine::Run()
{
	graphic->Frame();



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