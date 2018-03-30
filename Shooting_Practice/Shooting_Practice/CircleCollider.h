#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider();
	CircleCollider(float, float, float);
	~CircleCollider();
	
	void Init();
	float GetRad();
	void SetRad(float);

private:
	float rad;

	bool Check();
	void SelectionBullet();

};