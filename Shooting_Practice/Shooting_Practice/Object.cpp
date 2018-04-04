#include "Object.h"

Object::Object()
{
	QueryPerformanceFrequency(&freq);
	sprite = NULL;
	animation = NULL;
	audio = NULL;
	isPlayingAnime = false;
}


Object::~Object()
{
}

void Object::SetActiveState(bool active) {
	this->active = active;
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

void Object::SetPlayingAnime(bool a) {
	this->isPlayingAnime = a;
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

bool Object::GetPlayingAnime() {
	return isPlayingAnime;
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

Collider_Info Object::GetColl_Info() {
	Collider_Info a{
		cirCollider->GetPosX(),
		cirCollider->GetPosY(),
		cirCollider->GetRad(),
		active,
		tag
	};
	return a;
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
	if (tag == Player) {
		if ((posX + horizontal * speed > PLAYSCR_W - sprite->GetWidth()) || (posX + horizontal * speed < 0))
			return;
		else {
			posX += horizontal * speed;
			cirCollider->MoveX(posX + sprite->GetWidth() / 2);
		}
	}
	else {
		posX += horizontal * speed;
		cirCollider->MoveX(posX + sprite->GetWidth() / 2);
	}

	return;
}

void Object::MoveVertical()
{
	if (tag == Player) {
		if ((posY + vertical * speed > PLAYSCR_H - sprite->GetHeight()) || (posY + vertical * speed < 0))
			return;
		else {
			posY += vertical * speed;
			cirCollider->MoveY(posY + sprite->GetHeight()/2);
		}
	}
	else {
		posY += vertical * speed;
		cirCollider->MoveY(posY + sprite->GetHeight() / 2);
	}

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
	this->device = device;
	bool rs;

	this->active = active;
	SetHorizontal(0.0f);
	if (tag == Enemy || tag == BackGround) {
		SetVertical(1.0f);

	}
	else if (tag == Projectile) {
		SetVertical(-1.0f);
	}
	else {
		SetVertical(0.0f);
	}

	SetPosX(posX);
	SetPosY(posY);
	SetSpeed(speed - 8.0f);
	SetPivotX(this->posX + (width / 2));
	SetPivotY(this->posY + (height / 2));
	SetTag(tag);

	// Initialize Sprite.
	plane = new Sprite;
	rs = plane->Init(device, this->posX, this->posY, width, height, screenW, screenH, texPath);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Sprite.", L"Error", MB_OK);
		return false;
	}

	sprite = plane;

	explosion = new Sprite;
	rs = explosion->Init(device, this->posX, this->posY, 100.0f, 100.0f, 1024.0f, 768.0f, DESTROY_ANIME);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Sprite.", L"Error", MB_OK);
		return false;
	}

	cirCollider = new CircleCollider(posX + (width / 2), posY + (height / 2), 10.0f);
	if (!cirCollider) {
		MessageBox(NULL, L"Failed to Initialize Collider.", L"Error", MB_OK);
		return false;
	}

	cirCollider->SetRad(sprite->GetWidth() / 2);

	animation = new Animation;
	animation->Init();
	

	// Initialize Animation

	// Initialize Sound 

	return true;
}

void Object::Render(ID3D11DeviceContext *deviceContext)
{
	bool rs;
	this->deviceContext = deviceContext;

	rs = this->sprite->Render(deviceContext, this->posX, this->posY);
	if (!rs) {
		MessageBox(NULL, L"Failed to Rendering Object.", L"Error", MB_OK);
		return;
	}

	return;
}

void Object::PlayAnimation()
{
	sprite = explosion;
	this->SetPlayingAnime(true);

	return;
}

bool Object::Do_Animation(){
	return sprite->Animation(deviceContext, posX, posY);
}

void Object::EndAnimation() {
	SetPlayingAnime(false);

	sprite = plane;

	return;
}

void Object::ResetPosition() {
	if (tag == BackGround && posY >= PLAYSCR_H)
		posY = -768.0f;
	else if ( (tag == Enemy && posY >= PLAYSCR_H) || (tag == Projectile && posY <= 0))
		active = false;
	/*else if (tag == Projectile && posY <= 0)
		active = false;*/
	return;
}

void Object::ReceiveInput(Input &input)
{
	if (!this->active)
		return;

	if (input.IsSpacePressed()) {
		Shoot();
	}
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

	if (input.IsShiftPressed()) {
		speed = SHIFT_SPEED;
	}
	else
		speed = prevSpeed;

	return;
}

void Object::Shoot()
{
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

	// 밀리초 단위 
	float passedTime = (float)(nowTime.QuadPart - largeInt.QuadPart) / (float)freq.QuadPart * 1000;

	if ((int)passedTime >= COOLTIME) {
		QueryPerformanceCounter(&largeInt);
		return false;
	}

	return true;
}

void Object::CheckCollision()
{


	return;
}