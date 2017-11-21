#include "D3DClass.h"

D3DClass::D3DClass() 
{
	m_device = 0;
	m_deviceContext = 0;
	m_swapChain = 0;
	m_renderTrView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterstate = 0;
}

D3DClass::~D3DClass() 
{

}

HRESULT D3DClass::InitD3D(HWND hwnd, int width, int height, float screenNear, float screenFar, bool windowMode, bool vsync)  
{
	HRESULT hr;
	IDXGIFactory *factory;
	IDXGIAdapter *adapter;
	IDXGIOutput *adapterOutput;
	UINT numModes, numerator, denominator;
	DXGI_MODE_DESC *displayModeList;

	//=====================================
	// Set Display Refresh Rate.
	//=====================================

	// Create DXGI Factory. (DX Graphic Interface)
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
		return E_FAIL;

	// Create Apater about First(Monitor) Graphic Card Interface by use Factory.
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return E_FAIL;

	// Enumerate First Adapter about Display.
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
		return E_FAIL;

	// Get Modes number about DXGI_FORMAT_R8G8B8A8_UNORM
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
		return E_FAIL;

	// as possible, save every Combination about monitor and Graphic Card
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return E_FAIL;

	// Filed in List Structure in DisplayMode.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
		return E_FAIL;

	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (UINT)width && displayModeList[i].Height == (UINT)height)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	//================================
	// SwapChain
	//===============================

	//===============================
	// Depth-Stencil
	//===============================

	//========================
	// Rasterizer
	//========================

	//=======================
	// View Port 
	//=======================

	//====================
	// Matrix
	//====================



	return;
}

ID3D11Device* D3DClass::GetDevice() {
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext() {
	return m_deviceContext;
}

void D3DClass::Release() 
{
	if (m_device) m_device->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_renderTrView) m_renderTrView->Release();
	if (m_depthStencilBuffer) m_depthStencilBuffer->Release();
	if (m_depthStencilState) m_depthStencilState->Release();
	if (m_depthStencilView) m_depthStencilView->Release();
	if (m_rasterstate) m_rasterstate->Release();

	return;
}

XMMATRIX D3DClass::GetProjMatrix() {
	return m_projMatrix;
}

XMMATRIX D3DClass::GetWorldMatrix() {
	return m_worldMatirx;
}

XMMATRIX D3DClass::GetOrthoMatrix() {
	return m_orthoMatirx;
}

void D3DClass::StartScene(float, float, float, float) {

	return;
}
void D3DClass::EndScene() {

	return;
}