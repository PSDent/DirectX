#include "Engine.h"

Engine::Engine()
{
	graphic = 0;
}

Engine::~Engine() { }

bool Engine::InitEngine(HWND hWnd, int width, int height, char *modelPath)
{
	graphic = new Graphic;
	if (!graphic)
		return false;

	bool rs;
	rs = graphic->Init(width, height, hWnd, modelPath);
	if (!rs) {
		MessageBox(hWnd, L"Failed to Initialize DirectX 3D.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Engine::Release()
{
	graphic->Release();
	delete graphic;

	return;
}

void Engine::Frame() {
	graphic->Frame();

	return;
}