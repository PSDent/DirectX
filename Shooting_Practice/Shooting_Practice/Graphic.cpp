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
	camera->SetPos(0.0f, 0.0f, 0.0f);
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

bool Graphic::Frame(vector<Object> &obj)
{
	XMMATRIX worldMatrix, viewMatrix, projMatrix, orthoMatrix;
	bool rs;
	
	// Start Rendering
	d2d->Start();

	camera->Render();

	viewMatrix = camera->GetViewMatrix();
	worldMatrix = d2d->GetWorld();
	projMatrix = d2d->GetProj();
	orthoMatrix = d2d->GetOrtho();

	for (int i = 0; i < obj.size(); i++) {
		// Input the Object's Vertex data to Shader to Rendering. 
		rs = obj[i].GetSprite().Render(d2d->GetDeviceContext(), obj[i].GetPosX(), obj[i].GetPosY());
		if (!rs)
			return false;

		rs = shader->Render(d2d->GetDeviceContext(), obj[i].GetSprite().GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, obj[i].GetSprite().GetTexture());
		if (!rs)
			return false;
	}

	// End the Rendering 
	d2d->End();

	return true;
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