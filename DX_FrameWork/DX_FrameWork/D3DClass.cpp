#include "D3DClass.h"

D3DClass::D3DClass() {

}

D3DClass::~D3DClass() {

}

HRESULT D3DClass::InitD3D(HWND, int, int, float, float, bool, bool) {

	return S_OK;
}

ID3D11Device* D3DClass::GetDevice() {
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext() {
	return m_deviceContext;
}