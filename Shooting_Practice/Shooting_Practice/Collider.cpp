#include "Collider.h"

Collider::Collider() {
	posX = 0;
	posY = 0;
	collision_H = 0;
	collision_W = 0;
}

Collider::Collider(float x, float y, float width, float height) {
	posX = x;
	posY = y;
	collision_W = width;
	collision_H = height;
}

Collider::~Collider() {

}

bool Collider::CheckCollision(vector<Object> &v)
{


	return false;
}

void Collider::SelectionBullet()
{

	return;
}

void Collider::Release()
{

	return;
}