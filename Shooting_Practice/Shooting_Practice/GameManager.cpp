#include "GameManager.h"

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

bool GameManager::init()
{
	bool rs;

	spawnMgr = new SpawnManager;
	if (!spawnMgr)
		return false;

	rs = spawnMgr->init();
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize SpawnManager.", L"Error", MB_OK);
		return false;
	}

	taskMgr = new TaskManager;
	if (!taskMgr)
		return false;

	rs = taskMgr->Init();
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize TaskManager.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GameManager::Frame(vector<Object>& obj)
{
	Insertion(obj);
	vector<Collision_Info> temp = taskMgr->Frame();
	ApplyCollision(temp, obj);

	return;
}

void GameManager::Insertion(vector<Object>& obj)
{
	taskMgr->InsertTask_Player(obj[0].GetColl_Info());
	for (int i = 0; i < obj[0].GetProjectile().size(); ++i)
		taskMgr->InsertTask_Player(obj[0].GetProjectile()[i].GetColl_Info());

	for (int i = 1; i < obj.size(); ++i) {
		taskMgr->InsertTask_Enemy(obj[i].GetColl_Info());
	}
}

void GameManager::ApplyCollision(vector<Collision_Info> col, vector<Object>& obj)
{
	for (int i = 0; i < col.size(); i++) {
		if (col[i].to == Enemy && col[i].from == Projectile) {
			obj[col[i].to_index].SetActiveState(false);
			obj[col[i].to_index].PlayAnimation();
			obj[0].GetProjectile()[col[i].from_index].SetActiveState(false);
		}
		else if (col[i].to == Player) {
			obj[col[i].to_index].SetActiveState(false);
			obj[col[i].to_index].PlayAnimation();
		}
	}

	return;
}

bool GameManager::IsSpawnTime(){
	return spawnMgr->timer();
}

void GameManager::Spawn()
{
	spawnMgr->SpawnEnemy();

	return;
}

bool* GameManager::GetSpawnPoint()
{
	return spawnMgr->GetSpawnPoint();
}

void GameManager::Release()
{
	if (spawnMgr) {
		spawnMgr->Release();
		delete spawnMgr;
	}

	if (taskMgr) {
		taskMgr->Release();
		delete taskMgr;
	}

	return;
}