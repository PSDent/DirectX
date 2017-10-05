//---------------------------------------------------------------------------------
// File: D3DClass.cpp
//
// This Source is for Initialize DirectX Interface(COM object) or Class
//
// Copyright PSDent. All rights reserved.
//-------------------------------------------------------------------------------------
#include "D3DClass.h"
#include "WinMain.h"


D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	m_depthStencilBuffer = 0;
}

D3DClass::~D3DClass()
{
	Release();
}

bool D3DClass::InitD3D(HWND hwnd, int width, int height)
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_VIEWPORT m_viewPort;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Create SwapChain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !FULL_SCREEN;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create Device & Swa+pChain.
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		D3D_DRIVER_TYPE t_driverType = driverTypes[i];

		hr = D3D11CreateDeviceAndSwapChain(
			NULL,			   // Adapter (Graphic Card)
			t_driverType,      // Driver Type
			NULL,			   // Driver Type Software. Usually set with NULL.
			0,				   // Device Flag 
			featureLevels,     // FeatureLevel Array
			numFeatureLevels,  // FeatureLevel Count
			D3D11_SDK_VERSION, // SDK Version
			&swapChainDesc,    // Swapchain Description Struct
			&m_swapChain,      // Created Swapchain Object
			&m_device,         // Created Device Object
			&featureLevel,     // Created FeatureLevel Information
			&m_deviceContext   // Create DeviceContext Object
		);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create Device and Swapchain.", L"Opps!", MB_OK);
		return hr;
	}

	// Get BackBuffer Pointer
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Get Swapchain Buffer.", L"Opps!", MB_OK);
		return hr;
	}

	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create Render Target View.", L"Opps!", MB_OK);
		return hr;
	}

	// Release the backbuffer.
	pBackBuffer->Release();
	pBackBuffer = 0;

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);

	// ViewPort
	m_viewPort.Width = (FLOAT)width;
	m_viewPort.Height = (FLOAT)height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &m_viewPort);	
	return true;
}

void D3DClass::Release()
{
	if (m_swapChain) m_swapChain->Release();
	if (m_device) m_device->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_depthStencilState) m_depthStencilState->Release();
	if (m_depthStencilView) m_depthStencilView->Release();
	if (m_rasterState) m_rasterState->Release();
	if (m_depthStencilBuffer) m_depthStencilBuffer->Release();
	
	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

void D3DClass::ClearScene()
{
	float color[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_swapChain->Present(0, 0);
	return;
}
