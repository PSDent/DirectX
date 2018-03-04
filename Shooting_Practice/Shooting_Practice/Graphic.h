#pragma once
#include "D2D.h"
#include "Shader.h"

///////////////////
// CONSTANT 
const bool VSYNC = true;
const bool WINDOW_MODE = true;
const float SCR_NEAR = 0.1f;
const float SCR_FAR  = 1000.0f;

class Graphic
{
public:
	Graphic();
	~Graphic();
	
	void Init(HWND, int, int);
	void Frame();
	void Release();

private:
	D2D *d2d;
	Shader *shader;
};

