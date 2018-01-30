#pragma once
#include "D3DClass.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"

const bool VSYNC		= true;
const bool WINDOW		= true;
const float SCR_NEAR	= 0.1f;
const float SCR_FAR		= 1000.0f;

class Graphic
{
public:
	Graphic();
	~Graphic();

	bool Init(int, int, HWND, char*);
	void Release();
	bool Frame();

public:

private:
	bool Render(float);

private:
	D3DClass *m_D3D;
	Camera *m_Camera;
	Model *m_Model;
	Shader *m_Shader;
	Light *m_Light;
};