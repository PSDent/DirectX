#include "Graphic.h"


Graphic::Graphic()
{
}


Graphic::~Graphic()
{
}

void Graphic::Init(HWND hWnd, int width, int height)
{
	d2d = new D2D;
	if (!d2d)
		return;

	d2d->Init(hWnd, width, height, VSYNC, WINDOW_MODE, SCR_NEAR, SCR_FAR);

	shader = new Shader;
	if (!shader)
		return;

	shader->Init(d2d->GetDevice(), hWnd);

	return;
}

void Graphic::Frame()
{


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

	return;
}