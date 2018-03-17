#include "Engine.h"

Engine::Engine()
{
	graphic = NULL;
	input = NULL;
	hWnd = 0;
	done = false;
}


Engine::~Engine()
{
}


bool Engine::Init(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	bool rs;

	this->hWnd = hWnd;
	this->screenW = width;
	this->screenW = height;
	this->hInstance = hInstance;

	graphic = new Graphic;
	if (!graphic)
		return false;

	rs = graphic->Init(hWnd, width, height);
	if (!rs) {
		MessageBox(hWnd, L"Failed to Initialize Graphic System.", L"Error", MB_OK);
		return false;
	}

	input = new Input;
	if (!input)
		return false;

	rs = input->Init(hInstance, hWnd, width, height);
	if (!rs) {
		MessageBox(hWnd, L"Failed to Initialize Input System.", L"Error", MB_OK);
		return false;
	}

	CreateObject();

	return true;
}

void Engine::Run()
{
	MSG Message = { 0 };
	bool rs;

	// Game Loop
	while (!done)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		if (Message.message == WM_QUIT)
			done = true;
		else {
			rs = Frame();
			if (!rs) {
				MessageBox(hWnd, L"There is Something Problem that while Proccesing Frame.", L"Error", MB_OK);
				done = true;
			}
		}

		if (input->IsEscapePressed())
			done = true;
	}

	return;
}

bool Engine::Frame()
{
	bool rs;

	rs = input->Frame();
	if (FAILED(rs)) {
		MessageBox(hWnd, L"There is something problem in input system", L"Error", MB_OK);
		return false;
	}

	rs = graphic->Frame(plane, backGround, hWnd);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"There is something problem in Graphic system", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Engine::Release()
{
	if (graphic) {
		graphic->Release();
		delete graphic;
	}

	if (input) {
		input->Release();
		delete input;
	}

	return;
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
	temp_player.InitObject(graphic->GetDevice(), 512.0f, 700.0f, 50.0f, 50.0f, SPEED, screenW, screenH, "PLAYER", Player);
	plane.push_back(temp_player);

	for (int i = 1; i <= MAX_ENERMY; i++) {
		Object temp;
		temp.InitObject(graphic->GetDevice(), (0.0f + i) * 100.0f, 0.0f, 50.0f, 50.0f, SPEED, screenW, screenH, "ENERMY", Enermy_1);
		plane.push_back(temp);
	}

	for (int i = 0; i < MAX_BACKGROUND; i++) {
		Object temp_back;
		temp_back.InitObject(graphic->GetDevice(), 0.0f, (0.0f + i) * -768.0f, 700.0f, 768.0f, SPEED, screenW, screenH, "BACKGROUND", Background_Space);
		backGround.push_back(temp_back);
	}

	//}

	return;
}