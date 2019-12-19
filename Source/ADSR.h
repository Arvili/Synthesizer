/*
  ==============================================================================

    ADSR.h
    Created: 9 Nov 2019 2:10:39pm
    Author:  arvil

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================


class ADSr    : public Component
{
	
public:
	enum EnvelopeStage {
		env_off = 0,
		env_attack,
		env_decay,
		env_sustain,
		env_release
	};
    ADSr();
    ~ADSr();

	double Process();
	double calculateCof(double start, double target, double length);

	void updateAttack(double aLength);
	void updateDecay(double dLength);
	void updateSustain(double sLevel);
	void updateRelease(double rLength);
	void setSampleRate(double sampleRate);

	double getEnvStage();
	void setEnvStage(EnvelopeStage env);


private:
	int env_stage;
	double output;
	double attackLength;
	double decayLength;
	double sustainLevel;
	double releaseLength;
	double attackCof;
	double decayCof;
	double releaseCof;
	double currentSampleRate = 0;
	double maxVal = 1;
	double minVal = 0.0001;
	double multiplier;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSr)
};
