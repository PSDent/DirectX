#include "Timer.h"

Timer::Timer()
{
	fps = 0;
	count = 0;
}

Timer::~Timer()
{

}

bool Timer::Init()
{
	startTime = timeGetTime();

	return true;
}

void Timer::Frame()
{
	count++;

	if (timeGetTime() >= (startTime + 1000))
	{
		fps = count;
		count = 0;

		startTime = timeGetTime();
	}
	
}

int Timer::GetFps()
{
	return fps;
}

double Timer::GetDeltaTime() {
	return deltaTime;
}