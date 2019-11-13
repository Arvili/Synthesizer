/*
  ==============================================================================

    Filter.h
    Created: 4 Nov 2019 9:50:01pm
    Author:  arvil

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Filter    : public Component
{
public:
    Filter();
    ~Filter();
	double process(double inputValue);
	void setCutoff(double newCut);
	void setResonance(double newRes);
	void setMode(int newMode);
	void calculateFeedback();
	double getCutoff();

private:
	double cutoff;
	double resonance;
	int mode;
	double buf0;
	double buf1;
	double buf2;
	double buf3;
	double feedback;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
