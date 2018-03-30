#include "Collider.h"

Collider::Collider() {}

Collider::Collider(float x, float y) {
	posX = x;
	posY = y;
}

Collider::~Collider() {

}

//bool Collider::CheckCollision(vector<Object> &v)
//{
//
//
//	return false;
//}

void Collider::SelectionBullet()
{

	return;
}

void Collider::Release()
{

	return;
}

void Collider::MoveX(float x) {
	posX = x;
	return;
}

void Collider::MoveY(float y) {
	posY = y;
	return;
}

float Collider::GetPosX() {
	return posX;
}

float Collider::GetPosY() {
	return posY;
}