#include "Graphic.h"


Graphic::Graphic()
{
	d2d = NULL;
	camera = NULL;
	shader = NULL;
}


Graphic::~Graphic()
{
}

bool Graphic::Init(HWND hWnd, int width, int height)
{
	bool rs;

	timer = new Timer;
	rs = timer->Init();
	if (!rs)
		return false;

	////////////////
	// Direct 2D 
	////////////////
	d2d = new D2D;
	if (!d2d)
		return false;

	rs = d2d->Init(hWnd, width, height, VSYNC, WINDOW_MODE, SCR_NEAR, SCR_FAR);
	if (!rs)
		return false;

	/////////////////
	// Camera
	////////////////

	camera = new Camera;
	if (!camera)
		return false;

	// Camera z값 주의하시길.. 무조건 0 미만이여야함. 
	// 안 그러면 카메라의 z축과 스프라이트의 z축이 너무 가까워서 안보이거나 문제 발생
	// 카메라의 좌상단 좌표를 0, 0 으로 맞춘다.
	camera->SetPos(130.0f, height / 2.0f * -1.0f, -10.0f);
	camera->SetRot(0.0f, 0.0f, 0.0f);

	/////////////
	// Shader
	/////////////

	shader = new Shader;
	if (!shader)
		return false;

	rs = shader->Init(d2d->GetDevice(), hWnd);
	if (!rs)
		return false;

	return true;
}

bool Graphic::Frame(vector<Object> &obj, vector<Object> &background, HWND hWnd)
{
	XMMATRIX worldMatrix, viewMatrix, projMatrix, orthoMatrix;
	bool rs;

	timer->Frame();
	DisplayFps(hWnd, timer->GetFps());


	// Start Rendering
	d2d->Start();

	camera->Render();

	viewMatrix = camera->GetViewMatrix();
	worldMatrix = d2d->GetWorld();
	projMatrix = d2d->GetProj();
	orthoMatrix = d2d->GetOrtho();

	Rendering(obj, background, hWnd, worldMatrix, viewMatrix, orthoMatrix);

	// End the Rendering 
	d2d->End();

	return true;
}

void Graphic::Rendering(vector<Object> &plane, vector<Object> &background, HWND hWnd, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ObjectRender(background, worldMatrix, viewMatrix, orthoMatrix);               // BackGround
	ObjectRender(plane[0].GetProjectile(), worldMatrix, viewMatrix, orthoMatrix); // Bullet (Player)
	ObjectRender(plane, worldMatrix, viewMatrix, orthoMatrix);                    // Plane

	return;
}

void Graphic::ObjectRender(vector<Object> &obj, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	bool rs;

	// 1PI = 180'
	// Matrix Rotation은 radian값을 사용함. 
	//worldMatrix = XMMatrixRotationZ(XM_PI);

	for (int i = 0; i < obj.size(); i++) {
		if (obj[i].GetActiveState()) {

			obj[i].Movement();
			obj[i].ResetPosition();

			// Input the Object's Vertex data to Shader to Rendering. 
			obj[i].Render(d2d->GetDeviceContext());

			rs = shader->Render(d2d->GetDeviceContext(), obj[i].GetSprite().GetIndexCount(),
				worldMatrix, viewMatrix, orthoMatrix, obj[i].GetSprite().GetTexture());
			if (!rs) {
				MessageBox(NULL, L"Failed to Rendering Shader.", L"Error", MB_OK);
				return;
			}
		}
		else if (obj[i].GetPlayingAnime()){
			if (obj[i].Do_Animation())
				obj[i].SetPlayingAnime(false);

			rs = shader->Render(d2d->GetDeviceContext(), obj[i].GetSprite().GetIndexCount(),
				worldMatrix, viewMatrix, orthoMatrix, obj[i].GetSprite().GetTexture());
			if (!rs) {
				MessageBox(NULL, L"Failed to Rendering Shader.", L"Error", MB_OK);
				return;
			}
		}
	}

	return;
}

void Graphic::Release()
{
	if (d2d) {
		d2d->Release();
		delete d2d;
	}

	if (shader) {
		shader->Release();
		delete shader;
	}

	if (camera) {
		delete camera;
	}

	return;
}

ID3D11Device* Graphic::GetDevice()
{
	return d2d->GetDevice();
}

void Graphic::DisplayFps(HWND hWnd, int fps)
{
	TCHAR strFps[15];
	swprintf_s(strFps, L"FPS : %d", fps);
	SetWindowText(hWnd, strFps);

	return;
}