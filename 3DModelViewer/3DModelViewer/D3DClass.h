#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <Windows.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <DXGI.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3DClass
{
public:
	D3DClass();
	~D3DClass();
	bool InitD3D(HWND, int, int, bool, bool, float, float);

	void End();
	void Start();
	void Release();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	XMMATRIX GetWorld();
	XMMATRIX GetProj();
	XMMATRIX GetOrtho();
public:

private:
	D3D_DRIVER_TYPE m_driverType;
	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	ID3D11RenderTargetView *m_renderTargetView;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11Texture2D *m_depthBuf;
	ID3D11DepthStencilState *m_depthStencilState;
	ID3D11DepthStencilView *m_depthStencilView;
	ID3D11RasterizerState *m_rasterState;

	XMMATRIX m_worldMatrix;
	XMMATRIX m_projMatrix;
	XMMATRIX m_orthoMatrix;

	bool m_vsync;
private:
};