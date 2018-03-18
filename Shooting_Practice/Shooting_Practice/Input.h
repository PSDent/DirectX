#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <Windows.h>
#include <dinput.h>

const int KEYSCAN = 0x80;

class Input
{
public:
	Input();
	~Input();
	
	bool Init(HINSTANCE, HWND, int, int);
	void Release();
	bool Frame();

	// Key Checking Func
	bool IsEscapePressed();

	// Move key
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsSpacePressed();
	bool IsShiftPressed();

	// Mouse
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8 *directInput;
	IDirectInputDevice8 *keyboard;
	IDirectInputDevice8 *mouse;
	
	unsigned char keyState[256];
	DIMOUSESTATE mouseState;

	int scrWidth, scrHeight;
	int mouseX, mouseY;

};

