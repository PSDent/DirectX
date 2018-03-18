#include "Input.h"

Input::Input()
{
	directInput = 0;
	keyboard = 0;
	mouse = 0;
}


Input::~Input()
{
}

bool Input::Init(HINSTANCE hInstance, HWND hWnd, int scrWidth, int scrHeight)
{
	HRESULT rs;

	this->scrWidth = scrWidth;
	this->scrHeight = scrHeight;

	this->mouseX = 0;
	this->mouseY = 0;

	rs = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"Failed to Create Input Device.", L"Error", MB_OK);
		return false;
	}

	///////////////
	// Keyboard
	//////////////
	rs = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"Failed to Initialize Keyboard Device.", L"Error", MB_OK);
		return false;
	}

	rs = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(rs))
		return false;

	// Set Coorperative level
	rs = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"Failed to Set Cooperative Level on keyboard.", L"Error", MB_OK);
		return false;
	}

	// Acquire the keyboard.
	if (FAILED(keyboard->Acquire()))
	{
		if (S_FALSE == DIERR_NOTINITIALIZED)
		{
			MessageBox(0, L"keyboard is not Initailzied.", L"Error", MB_OK);
			return false;
		}
	}

	/////////////
	// Mouse
	/////////////
	rs = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"Failed to Initialize Mouse Device.", L"Error", MB_OK);
		return false;
	}

	rs = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(rs))
		return false;

	// Set Coorperative level
	rs = mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(rs)) {
		MessageBox(hWnd, L"Failed to Set Cooperative Level on Mouse.", L"Error", MB_OK);
		return false;
	}


	if (FAILED(mouse->Acquire()))
	{
		if (S_FALSE == DIERR_NOTINITIALIZED)
		{
			MessageBox(0, L"mouse is not Initailzied.", L"Error", MB_OK);
			return false;
		}
	}

	return true;
}

void Input::Release()
{
	if (mouse) {
		mouse->Unacquire();
		mouse->Release();
	}

	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
	}

	if (directInput) {
		directInput->Release();
	}

	return;
}

bool Input::Frame()
{
	bool rs;

	rs = ReadKeyboard();
	if (!rs) {
		return false;
	}

	rs = ReadMouse();
	if (!rs) {
		return false;
	}

	ProcessInput();

	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT rs;

	rs = keyboard->GetDeviceState(sizeof(keyState), (LPVOID)&keyState);
	if (FAILED(rs)) {
		if ((rs == DIERR_INPUTLOST) || (rs == DIERR_NOTACQUIRED))
			keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT rs;

	rs = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(rs)) {
		if ((rs == DIERR_INPUTLOST) || (rs == DIERR_NOTACQUIRED))
			mouse->Acquire();
		else
			return false;
	}

	return true;
}

void Input::ProcessInput()
{
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;

	if (mouseX < 0) mouseX = 0;
	if (mouseY < 0) mouseY = 0;

	if (mouseX > scrWidth) mouseX = scrWidth;
	if (mouseY > scrHeight) mouseY = scrHeight;

	return;
}

void Input::GetMouseLocation(int &mouseX, int &mouseY)
{
	mouseX = this->mouseX;
	mouseY = this->mouseY;

	return;
}


bool Input::IsEscapePressed(){
	if (keyState[DIK_ESCAPE] & KEYSCAN) 
		return true;
	return false;
}

bool Input::IsLeftPressed(){
	if (keyState[DIK_LEFT] & KEYSCAN)
		return true;
	return false;
}

bool Input::IsRightPressed(){
	if (keyState[DIK_RIGHT] & KEYSCAN)
		return true;
	return false;
}
bool Input::IsUpPressed() {
	if (keyState[DIK_UP] & KEYSCAN)
		return true;
	return false;
}

bool Input::IsDownPressed() {
	if (keyState[DIK_DOWN] & KEYSCAN)
		return true;
	return false;
}

bool Input::IsSpacePressed() {
	if (keyState[DIK_SPACE] & KEYSCAN)
		return true;
	return false;
}

bool Input::IsShiftPressed() {
	if (keyState[DIK_LSHIFT] & KEYSCAN)
		return true;
	return false;
}

