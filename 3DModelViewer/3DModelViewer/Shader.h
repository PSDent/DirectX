#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class Shader
{
public:
	// It must be same with Constant Buffer in Shader.
	struct MatrixBufType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

	struct LightBufType {
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;    // multiple of 16 
	};

public:
	Shader();
	~Shader();

	bool Init(ID3D11Device*, HWND);
	void Release();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3 ,  XMFLOAT4 );

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ReleaseShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParam(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader *m_VS;
	ID3D11PixelShader *m_PS;
	ID3D11InputLayout *m_layout;
	ID3D11Buffer *m_matrixBuf;
	ID3D11SamplerState *m_sampleState;
	ID3D11Buffer *m_lightBuf;

};