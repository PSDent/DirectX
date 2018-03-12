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
	bool rs;

	this->posX = posX;
	this->posY = posY;
	// Initialize Sprite.
	sprite = new Sprite;
	rs = sprite->Init(device, this->posX, this->posY, width, height, texPath);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Sprite.", L"Error", MB_OK);
		return false;
	}
	

	// Initialize Animation

	// Initialize Sound 


	return true;
}
