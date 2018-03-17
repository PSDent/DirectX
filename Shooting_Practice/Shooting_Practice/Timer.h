#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

class Timer {
public:
	Timer();
	~Timer();

	bool Init();
	void Frame();

	int GetFps();
	double GetDeltaTime();

private:
	int fps, count;
	unsigned long startTime;
	double deltaTime;

};