#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Texture.h"

using namespace DirectX;
using namespace std;

class Model
{
public:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 tex;
		XMFLOAT3 normal;
	};

	struct ModelType {
		float x, y, z;
		float u, v;
		float nx, ny, nz;
	};

public:
	Model();
	~Model();

	bool Init(ID3D11Device*, WCHAR*, char*);
	void Release();
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	bool InitBuf(ID3D11Device*);
	void ReleaseBuf();
	void RenderBuf(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuf, *m_indexBuf;
	int m_vertexCount, m_indexCount;
	Texture *m_Texture;
	ModelType *m_model;

};