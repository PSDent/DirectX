#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:
	Light();
	~Light();

	void SetDiffuseColor(float, float, float, float);
	void SetDir(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDir();

public:

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_dir;

private:
};