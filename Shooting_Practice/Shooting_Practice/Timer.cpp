#include "Timer.h"

Timer::Timer()
{

}

Timer::~Timer()
{

}

bool Timer::Init()
{
	fps = 0;
	count = 0;
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