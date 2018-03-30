#include "SpawnManager.h"
#include "TaskManager.h"
#include "Object.h"
#include "Common.h"

class GameManager {
public:
	GameManager();
	~GameManager();
	
	void Frame(vector<Object>& a);
	void Release();
	bool init();
	bool* GetSpawnPoint();
	bool IsSpawnTime();
	void Spawn();

private:
	void ApplyCollision(vector<Collision_Info>, vector<Object>&);
	void Insertion(vector<Object>&);

private:
	SpawnManager *spawnMgr;
	TaskManager *taskMgr;

};