#include "Light.h"

Light::Light()
{
}

Light::~Light()
{

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

XMFLOAT4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 Light::GetDir()
{
	return m_dir;
}