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

	return true;
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

	return;
}