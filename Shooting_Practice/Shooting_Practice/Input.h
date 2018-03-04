#pragma once
#include <Windows.h>

class Input
{
public:
	Input();
	~Input();

	LRESULT MsgHandle(HWND, UINT, WPARAM, LPARAM);

private:
};

