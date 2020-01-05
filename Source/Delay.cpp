

#include "../JuceLibraryCode/JuceHeader.h"
#include "Delay.h"

Delay::Delay()
{
	rPointer = 1;
	wPointer = 0;
	
	circularRigthBuffer = (float*)calloc(2000, sizeof(float));
	circularLeftBuffer = (float*)calloc(2000, sizeof(float));
	
}

Delay::~Delay()
{
}

void Delay::addDelay(float* rigthBuffer, float* leftBuffer)
{
	float rigthSample;
	float leftSample;
	for (int i = 0; i < 2000; i++)
	{
		circularRigthBuffer[wPointer] = rigthBuffer[i];
		rigthSample = rigthBuffer[i] + circularRigthBuffer[rPointer];
		//rigthBuffer[i] =  rigthSample;
		circularLeftBuffer[wPointer] = leftBuffer[i];
		leftSample = leftBuffer[i] + circularLeftBuffer[rPointer];
		//leftBuffer[i] = leftSample;
		rPointer = fmod(rPointer, 2000) + 1;
		wPointer = fmod(wPointer, 2000) + 1;
	}
}

bool Delay::isOn()
{
	return on;
}