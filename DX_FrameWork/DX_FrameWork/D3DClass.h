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
	HRESULT InitD3D();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext GetDeviceContext();


private:
};