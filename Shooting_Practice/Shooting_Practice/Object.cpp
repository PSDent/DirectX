#include "Object.h"

Object::Object()
{
	QueryPerformanceFrequency(&freq);
	sprite = NULL;
	animation = NULL;
	audio = NULL;
	isTimerOn = false;
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

bool Object::GetActiveState() {
	return active;
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

vector<Object>& Object::GetProjectile() {
	return projectile;
}

void Object::MoveHorizon()
{
	if (tag == "PLAYER") {
		if ((posX + horizontal * speed > PLAYSCR_W - sprite->GetWidth()) || (posX + horizontal * speed < 0))
			return;
		else
			posX += horizontal * speed;
	}
	else
		posX += horizontal * speed;

	return;
}

void Object::MoveVertical()
{
	if (tag == "PLAYER") {
		if ((posY + vertical * speed > PLAYSCR_H - sprite->GetHeight()) || (posY + vertical * speed < 0))
			return;
		else
			posY += vertical * speed;
	}
	else
		posY += vertical * speed;

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

bool Object::InitObject(ID3D11Device* device, bool active, float posX, float posY, float width, float height, float speed, int screenW, int screenH, const string tag, const WCHAR *texPath)
{
	bool rs;

	this->active = active;
	SetHorizontal(0.0f);
	if (tag == "ENERMY" || tag == "BACKGROUND")
		SetVertical(1.0f);
	else if (tag == "PROJECTIONTILE")
		SetVertical(-1.0f);
	else
		SetVertical(0.0f);
	SetPosX(posX);
	SetPosY(posY);
	SetSpeed(speed - 8.0f);
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
	if (tag == "BACKGROUND" && posY >= PLAYSCR_H)
		posY = -768.0f;
	else if (tag == "PROJECTIONTILE" && posY <= 0)
		active = false;

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
		Shoot();
	}

	if (input.IsShiftPressed()) {
		speed = SHIFT_SPEED;
	}
	else
		speed = prevSpeed;


	return;
}

void Object::Shoot()
{
	if (!isTimerOn) {
		BeginTimer();
		isTimerOn = true;
	}

	if (!IsCooltime()) {
		// Left Missile
		projectile[projectileCnt].active = true;
		projectile[projectileCnt].posX = this->posX;
		projectile[projectileCnt].posY = this->posY;

		// Right Missile
		projectile[projectileCnt + 1].active = true;
		projectile[projectileCnt + 1].posX = this->posX + GAP;
		projectile[projectileCnt + 1].posY = this->posY;

		if (projectileCnt < 28)
			projectileCnt += 2;
		else
			projectileCnt = 0;
	}
	
	return;
}

bool Object::IsCooltime()
{
	LARGE_INTEGER nowTime;

	QueryPerformanceCounter(&nowTime);
	int mod = (nowTime.QuadPart - largeInt.QuadPart) / freq.QuadPart / 1000 % COOLTIME;

	if (!mod) {
		return false;
	}

	return true;
}

void Object::BeginTimer()
{
	QueryPerformanceCounter(&largeInt);

	return;
}