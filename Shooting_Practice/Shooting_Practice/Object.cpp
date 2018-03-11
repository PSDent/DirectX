#include "Object.h"



Object::Object()
{
	sprite = NULL;
	animation = NULL;
	audio = NULL;
}


Object::~Object()
{
}

float Object::GetPosX()
{
	return this->posX;
}


float Object::GetPosY()
{
	return this->posY;
}

Sprite& Object::GetSprite() {
	return *sprite;
}

Animation& Object::GetAnime() {
	return *animation;
}

Audio& Object::GetAudio() {
	return *audio;
}

bool Object::InitObject(ID3D11Device* device, int posX, int posY, int width, int height, WCHAR *texPath)
{
	// Initialize Sprite.
	sprite->Init(device, posX, posY, width, height, texPath);

	// Initialize Animation

	// Initialize Sound 


	return true;
}
