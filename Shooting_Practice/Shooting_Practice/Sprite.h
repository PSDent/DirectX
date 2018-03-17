#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Texture.h"

using namespace DirectX;
using namespace std;

class Sprite
{
public:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 tex;
	};

public:
	Sprite();
	~Sprite();

	bool Init(ID3D11Device*, int, int, int, int, int, int, const WCHAR*);
	void Release();
	bool Render(ID3D11DeviceContext*, int, int);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();
	void Rotate(float);

private:
	bool InitBuf(ID3D11Device*);
	void ReleaseBuf();
	void RenderBuf(ID3D11DeviceContext*);
	bool UpdateBuf(ID3D11DeviceContext*, int, int);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer * m_vertexBuf, *m_indexBuf;
	int m_vertexCount, m_indexCount;
	Texture *m_Texture;

	// sprite's information to draw 2D image
	int m_scrX, m_scrY;
	int m_spriteW, m_spriteH;
	int m_prevX, m_prevY;
	int m_screenW, m_screenH;

};