#include "SpawnManager.h"

class GameManager {
public:
	GameManager();
	~GameManager();
	
	void Release();
	bool init();
	void Spawn();
	bool* GetSpawnPoint();
	bool IsSpawnTime();

private:

private:
	SpawnManager *spawnMgr;

};