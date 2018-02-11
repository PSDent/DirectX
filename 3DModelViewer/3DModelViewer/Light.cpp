#include "Light.h"

Light::Light()
{}

Light::~Light()
{}

void Light::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = XMFLOAT4(r, g, b, a);
	return;
}

void Light::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = XMFLOAT4(r, g, b, a);
	return;
}

void Light::SetDir(float r, float g, float b)
{
	m_dir = XMFLOAT3(r, g, b);
	return;
}

void Light::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

void Light::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = XMFLOAT4(r, g, b, a);
	return;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return m_specularColor;
}

float Light::GetSpecularPower()
{
	return m_specularPower;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 Light::GetDir()
{
	return m_dir;
}

XMFLOAT4 Light::GetAmbientColor()
{
	return m_ambientColor;
}