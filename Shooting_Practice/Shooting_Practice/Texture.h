#pragma once
#include <D3D11.h>
#include <D3DX11tex.h>

class Texture
{
public:
	Texture();
	~Texture();

	bool Init(ID3D11Device*, const WCHAR*);
	void Release();

	ID3D11ShaderResourceView* GetTexPtr();

public:

private:
	ID3D11ShaderResourceView* m_texture;

private:

};