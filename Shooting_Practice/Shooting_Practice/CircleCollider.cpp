#include "CircleCollider.h"

CircleCollider::CircleCollider() : Collider()
{
}

CircleCollider::CircleCollider(float posX, float posY, float rad)
	: Collider(posX, posY), rad(rad)
{}

CircleCollider::~CircleCollider()
{

}

void CircleCollider::Init()
{


	return;
}

bool CircleCollider::Check()
{


	return false;
}
void CircleCollider::SelectionBullet()
{

	return;
}

void CircleCollider::SetRad(float r)
{
	this->rad = r;
	return;
}

float CircleCollider::GetRad() {
	return rad;
}
