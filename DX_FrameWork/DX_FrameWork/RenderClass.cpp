#include "RenderClass.h"

RenderClass::RenderClass() {
	d3d = 0;
}

RenderClass::~RenderClass() {

}

HRESULT RenderClass::Init(HWND hwnd, int width, int height, bool windowMode) {

	d3d = new D3DClass;
	if (FAILED(d3d))
		return E_FAIL;

	d3d->InitD3D(hwnd, width, height, screenNear, screenFar, windowMode, vsync);


	return S_OK;
}

void RenderClass::Frame() {

	return;
}

void RenderClass::Release() {
	
	return;
}