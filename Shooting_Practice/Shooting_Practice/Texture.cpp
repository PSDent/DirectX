#include "Texture.h"

Texture::Texture()
{
	m_texture = 0;
}

Texture::~Texture() {}

bool Texture::Init(ID3D11Device* device, const WCHAR* texPath)
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile(device, texPath, NULL, NULL, &m_texture, NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Texutre Resource", L"Error", MB_OK);
		return false;
	}

	return true;
}
void Texture::Release()
{
	if (m_texture) m_texture->Release();

	return;
}

ID3D11ShaderResourceView* Texture::GetTexPtr()
{
	return m_texture;
}