
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Delay    : public Component
{
public:
    Delay();
    ~Delay();

	void addDelay(float* rightBuffer, float* leftBuffer);
	bool isOn();
	

private:
	float* circularRigthBuffer;
	float* circularLeftBuffer;
	int rPointer;
	int wPointer;
	bool on = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)

};
