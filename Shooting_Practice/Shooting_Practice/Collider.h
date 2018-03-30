#pragma once

#include <DirectXMath.h>
#include "Common.h"

class Collider
{
public:
	Collider();
	Collider(float, float);
	
	void Release();
	virtual ~Collider();

	void MoveX(float);
	void MoveY(float);

	float GetPosX();
	float GetPosY();

private:
	virtual bool Check() = 0;
	void SelectionBullet();

private:
	float posX, posY;
};