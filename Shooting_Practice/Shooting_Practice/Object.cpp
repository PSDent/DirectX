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


void Object::SetVertical(float vertical) {
	this->vertical = vertical;
}


void Object::SetHorizontal(float horizontal)
{
	this->horizontal = horizontal;
}


void Object::SetPivotX(float x){
	this->pivotX = x;
}


void Object::SetPivotY(float y){
	this->pivotY = y;
}


void Object::SetPosX(float x) {
	this->posX = x;
}

void Object::SetPosY(float y) {
	this->posY = y;
}

string Object::GetTag() {
	return this->tag;
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

void Object::MoveHorizon()
{
	this->posX += this->horizontal * (this->speed);

	return;
}

void Object::MoveVertical()
{
	this->posY += this->vertical * (this->speed);

	return;
}

void Object::Movement()
{
	MoveHorizon();
	MoveVertical();

	return;
}

void Object::SetTag(string tag) {
	this->tag = tag;
}

void Object::SetSpeed(float speed) {
	this->speed = speed;
	prevSpeed = speed;
}

bool Object::InitObject(ID3D11Device* device, float posX, float posY, float width, float height, float speed, int screenW, int screenH, const string tag, const WCHAR *texPath)
{
	bool rs;

	SetHorizontal(0.0f);
	if (tag == "ENERMY" || tag == "BACKGROUND")
		SetVertical(1.0f);
	else
		SetVertical(0.0f);
	SetPosX(posX);
	SetPosY(posY);
	SetSpeed(speed);
	SetPivotX(this->posX + (width / 2));
	SetPivotY(this->posY + (height / 2));
	SetTag(tag);

	// Initialize Sprite.
	sprite = new Sprite;
	rs = sprite->Init(device, this->posX, this->posY, width, height, screenW, screenH, texPath);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Sprite.", L"Error", MB_OK);
		return false;
	}
	

	// Initialize Animation

	// Initialize Sound 

	return true;
}

void Object::Render(ID3D11DeviceContext *deviceContext)
{
	bool rs;

	rs = this->sprite->Render(deviceContext, this->posX, this->posY);
	if (!rs) {
		MessageBox(NULL, L"Failed to Rendering Object.", L"Error", MB_OK);
		return;
	}

	return;
}

void Object::ResetPosition() {
	if (tag == "BACKGROUND" && posY >= 768.0f)
		posY = -768.0f;

	return;
}

void Object::ReceiveInput(Input &input)
{
	if (input.IsLeftPressed()) {
		if (horizontal >= -MAX_HORIZONTAL)
			horizontal -= INCREASE_HORIZONTAL;
	}
	else if (input.IsRightPressed()) {
		if (horizontal <= MAX_HORIZONTAL)
			horizontal += INCREASE_HORIZONTAL;
	}
	else
		horizontal = 0.0f;

	if (input.IsUpPressed()) {
		if (vertical >= -MAX_VERTICAL)
			vertical -= INCREASE_VERTICAL;
	}
	else if (input.IsDownPressed()) {
		if (vertical <= MAX_VERTICAL)
			vertical += INCREASE_VERTICAL;
	}
	else
		vertical = 0.0f;

	if (input.IsSpacePressed()) {

	}

	if (input.IsShiftPressed()) {
		speed = SHIFT_SPEED;
	}
	else
		speed = prevSpeed;


	return;
}