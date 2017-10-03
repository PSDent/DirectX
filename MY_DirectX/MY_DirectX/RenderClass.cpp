//---------------------------------------------------------------------------------
// File: RenderClass.cpp
//
// It is exist to rendering and capsulation.
//
// Copyright PSDent. All rights reserved.
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

	if (!d3dclass->InitD3D(hwnd, width, height))
		return false;

	return true;
}

void RenderClass::Update()
{
	d3dclass->ClearScene();
	
	return;
}

void RenderClass::Release()
{
	delete renderclass;
	delete d3dclass;
}