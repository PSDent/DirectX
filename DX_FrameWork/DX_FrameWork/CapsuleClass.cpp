#include "CapsuleClass.h"

CapsuleClass::CapsuleClass() {
	render = 0;
	input = 0;
}

CapsuleClass::~CapsuleClass() {

}

HRESULT CapsuleClass::InitEngine(HWND hwnd, int width, int height, bool windowMode) {
	HRESULT hr;

	//================
	// RenderClass
	//================
	render = new RenderClass; 
	if (render)
		return E_FAIL;

	render->Init(hwnd, width, height, windowMode, );

	//==================
	// InputClass
	//==================

	input = new InputClass;
	if (input)
		return E_FAIL;

	input->Init();

	return S_OK;
}

void CapsuleClass::Release() {

	return;
}

void CapsuleClass::Frame() {

	return;
}