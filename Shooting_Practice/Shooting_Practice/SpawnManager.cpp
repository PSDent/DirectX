#include "SpawnManager.h"

SpawnManager::SpawnManager()
{
}

SpawnManager::~SpawnManager()
{

}

void SpawnManager::SpawnEnemy()
{
	//if (timer()) {
		ReadStageLine();
	//}

	return;
}

bool* SpawnManager:: GetSpawnPoint() {
	return spawnPoint;
}

void SpawnManager::ReadStageLine()
{
	char input;
	for (int i = 0; i < SPAWN_NUM;) {
		if (fin->eof())
			return;

		fin->get(input);
		if (input == '0') 
			spawnPoint[i] = false;
		else if(input == '1') 
			spawnPoint[i] = true;

		++i;
	}

	return;
}

bool SpawnManager::init()
{
	QueryPerformanceFrequency(&freq);
	
	fin = new fstream;
	if (!fin) {
		return false;
	}
	
	fin->open(STAGE1);
	if (fin->fail()) {
		MessageBox(NULL, L"Failed to Open the Stage File. ", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool SpawnManager::timer()
{
	LARGE_INTEGER nowTime;

	QueryPerformanceCounter(&nowTime);

	// 밀리초 단위 
	float passedTime = (float)(nowTime.QuadPart - largeInt.QuadPart) / (float)freq.QuadPart * 1000;

	if ((int)passedTime >= SPAWN_TIME) {
		QueryPerformanceCounter(&largeInt);
		return true;
	}

	return false;
}

void SpawnManager::Release() {

	if (fin) {
		fin->close();
		delete fin;
	}

	return;
}