#pragma once
#include "D3DClass.h"
#include "Graphic.h"

const bool windowMode = true;

class Engine {
public:
	Engine();
	~Engine();

	bool InitEngine(HWND, int, int, char*);
	void Frame();
	void Release();

private:
	Graphic *graphic;
};