#pragma once
#include <vector>
#include "D2D.h"
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Object.h"

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
	
	bool Init(HWND, int, int);
	bool Frame(vector<Object>&, vector<Object>&, HWND);
	void Release();
	void DisplayFps(HWND, int);
	ID3D11Device* GetDevice();

private:
	void Rendering(vector<Object>&, vector<Object>&, HWND, XMMATRIX, XMMATRIX, XMMATRIX);
	void ObjectRender(vector<Object> &obj, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	D2D *d2d;
	Shader *shader;
	Camera *camera;
	Timer *timer;

};

