#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:
	Light();
	~Light();

	void SetSpecularPower(float);
	void SetSpecularColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDir(float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDir();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

public:

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_dir;
	XMFLOAT4 m_specularColor;
	float m_specularPower;

private:
};