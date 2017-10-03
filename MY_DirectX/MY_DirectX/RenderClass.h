//---------------------------------------------------------------------------------
// File: RenderClass.h
//
// Copyright PSDent. All rights reserved.
//-------------------------------------------------------------------------------------
#pragma once
#include "D3DClass.h"

class RenderClass {
public:
	RenderClass* renderclass;
	D3DClass *d3dclass;

public:
	RenderClass();
	~RenderClass(); 

	void Release();
	bool Setup(HWND hwnd, int, int);
	void Update();
};