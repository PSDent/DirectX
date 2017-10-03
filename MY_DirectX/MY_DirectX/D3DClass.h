//---------------------------------------------------------------------------------
// File: D3DClass.h
//
// Copyright PSDent. All rights reserved.
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

class D3DClass
{
public:
	D3DClass();
	~D3DClass();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	bool InitD3D(HWND, int, int);
	void Release();
	void ClearScene();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11Texture2D* m_depthStencilBuffer;


};
