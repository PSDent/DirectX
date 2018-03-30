#include "Engine.h"

Engine::Engine()
{
	graphic = NULL;
	input = NULL;
	hWnd = 0;
	done = false;
	gameMgr = NULL;
	spawnPoint = NULL;
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

	gameMgr = new GameManager;
	if (!gameMgr)
		return false;
	rs = gameMgr->init();
	if (!rs) {
		MessageBox(hWnd, L"Failed to Initialize GameManager System.", L"Error", MB_OK);
		return false;
	}

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

	// Check Input State
	ApplyInput(); 
	
	// Spawn Enemy in stage.
	Spawn();

	gameMgr->Frame(plane);

	rs = graphic->Frame(plane, backGround, hWnd);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"There is something problem in Graphic system", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Engine::Spawn()
{
	if (!gameMgr->IsSpawnTime())
		return;

	gameMgr->Spawn();
	spawnPoint = gameMgr->GetSpawnPoint();

	for (int i = 0; i < SPAWN_POINT; i++) {
		if (spawnPoint[i]) {
			int j = 0;
			while (j + 1 <= MAX_ENEMY && plane[++j].GetActiveState());
			
			if (!plane[j].GetActiveState()) {
				plane[j].SetActiveState(true);
				plane[j].SetPosX(i * TERM);
				plane[j].SetPosY(0.0f);

				TCHAR strFps[15];
				swprintf_s(strFps, L"Enemy : %d", j);
				SetWindowText(hWnd, strFps);
			}
		}
	}

	return;
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

	if (gameMgr) {
		gameMgr->Release();
		delete gameMgr;
	}

	return;
}

void Engine::ApplyInput()
{
	plane[0].ReceiveInput(*input);

	return;
}

bool Engine::IsWin() {

	return true;
}

void Engine::CreateObject()
{
	// 0 ~ 800 ÀÎ°ÔÀÓ 801~1024 UI 
	// 1024 x 768
	Object temp_player;
	temp_player.InitObject(graphic->GetDevice(), true, 512.0f, 700.0f, 50.0f, 50.0f, SPEED, screenW, screenH, Player, PLAYER);
	plane.push_back(temp_player);
	
	for (int i = 0; i < MAX_PROJECTION; i++) {
		Object temp_projectile;
		temp_projectile.InitObject(graphic->GetDevice(), false, 0.0f, 0.0f, 9.0f, 54.0f, SPEED + 6.0f, screenW, screenH, Projectile, PROJECTION_TILE);
		plane[0].GetProjectile().push_back(temp_projectile);
	}
	for (int i = 1; i <= MAX_ENEMY; i++) {
		Object temp;
		temp.InitObject(graphic->GetDevice(), false, 0.0f, 0.0f, 50.0f, 50.0f, SPEED + PLANE_ADDITION, screenW, screenH, Enemy, ENERMY_1);
		plane.push_back(temp);
	}

	for (int i = 0; i < MAX_BACKGROUND; i++) {
		Object temp_back;
		temp_back.InitObject(graphic->GetDevice(), true, 0.0f, (0.0f + i) * -768.0f, 700.0f, 768.0f, SPEED + 4.0f, screenW, screenH, BackGround, BACKGROUND_SPACE);
		backGround.push_back(temp_back);
	}

	//}

	return;
}

