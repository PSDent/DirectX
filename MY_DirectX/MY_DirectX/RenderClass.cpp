//---------------------------------------------------------------------------------
// File: RenderClass.cpp
//
// It is exist to rendering and capsulation.
//-------------------------------------------------------------------------------------
#include "RenderClass.h"
#include "D3DClass.h"

RenderClass::RenderClass()
{

}

RenderClass::~RenderClass()
{
	
}

bool RenderClass::Setup(HWND hwnd, int width, int height)
{
	d3dclass = new D3DClass;

	if (!d3dclass->InitD3D(hwnd, width, height, SCREEN_NEAR, SCREEN_FAR, VSYNC, WINDOW_MODE))
	{
		MessageBox(hwnd, L"Failed Initialize Direct Object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void RenderClass::Update()
{
	d3dclass->ClearScene();
	
	return;
}

void RenderClass::Release()
{
	delete d3dclass;
}