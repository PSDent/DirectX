#include <fstream>
#include "ResourcePath.h"

using namespace std;

const float SPAWN_TIME = 500.0f;
const float SPAWN_NUM = 9;

class SpawnManager
{
public:
	SpawnManager();
	~SpawnManager();

	void Release();
	bool init();
	void SpawnEnemy();
	bool* GetSpawnPoint();
	bool timer();

private:
	void ReadStageLine();

private:
	LARGE_INTEGER largeInt, freq;
	fstream *fin;
	char strBuf[10];
	bool spawnPoint[9];
};