//---------------------------------------------------------------------------------
// File: D3DClass.h
//-------------------------------------------------------------------------------------
#pragma once

////////////
// LINKING
////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

///////////
// INCLUDE 
///////////

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3DClass
{
public:
	D3DClass();
	~D3DClass();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	bool InitD3D(HWND, int, int, float, float, bool, bool);
	void Release();
	void ClearScene();

	XMMATRIX GetProjMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthoMatrix();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11Texture2D* m_depthStencilBuffer;

	XMMATRIX m_projMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};
