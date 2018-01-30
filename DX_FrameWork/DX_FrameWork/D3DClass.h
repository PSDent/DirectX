#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

using namespace DirectX;

class D3DClass {
public:
	D3DClass();
	~D3DClass();

	HRESULT InitD3D(HWND, int, int, float, float, bool, bool);
	void Release();

	void StartScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	XMMATRIX GetProjMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthoMatrix();

private:
	XMMATRIX m_projMatrix;
	XMMATRIX m_worldMatirx;
	XMMATRIX m_orthoMatirx;

private:
	bool m_vsync;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	IDXGISwapChain *m_swapChain;
	ID3D11RenderTargetView *m_renderTrView;

	ID3D11Texture2D *m_depthStencilBuffer;
	ID3D11DepthStencilState *m_depthStencilState;
	ID3D11DepthStencilView *m_depthStencilView;

	ID3D11RasterizerState *m_rasterstate;


};