/*
  ==============================================================================

    ADSR.cpp
    Created: 9 Nov 2019 2:10:39pm
    Author:  arvil

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSR.h"
#include<math.h>
#include <iostream>

//==============================================================================

ADSr::ADSr()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	output = 0;
	env_stage = env_off;
	attackLength = 1;
	decayLength = 0.2;
	sustainLevel = 1;
	releaseLength = 0.2;
	currentSampleRate = 44100;
	updateAttack(attackLength);
	updateDecay(decayLength);
	updateRelease(releaseLength);
}



ADSr::~ADSr()
{
}

double ADSr::Process()
{
	switch (env_stage) {
		case env_off:
			break;
		case env_attack:
			output = attackCof + output;
			if (output > 1)
			{	
				env_stage = env_decay;
				output = 1;
			}
			break;
		case env_decay:
			output = decayCof*output;
			if (output < sustainLevel)
			{
				output = sustainLevel;
				env_stage = env_sustain;
			}
			break;
		case env_sustain:
			output = sustainLevel;
			break;
		case env_release:
			//output = releaseCof * output;
			output = releaseCof*output;
			if (output < minVal)
			{
				output = 0;
				env_stage = env_off;
			}
			break;

		
	}	
	return output;
}

double ADSr::calculateCof(double start, double target, double length)
{
	multiplier = 1.0 + (log(target) - log(start)) / (length);
	return multiplier;
}

void ADSr::updateAttack(double aLength)
{
	attackLength = (minVal+aLength)*currentSampleRate;
	attackCof = 1 / attackLength;
	
	
}

void ADSr::updateDecay(double dLength) {
	decayLength = (minVal+dLength)*currentSampleRate;
	decayCof = calculateCof(maxVal, sustainLevel, decayLength);
}

void ADSr::updateSustain(double sLevel)
{
	sustainLevel = sLevel;
}

void ADSr::updateRelease(double rLength) 
{
	releaseLength = (minVal+rLength)*currentSampleRate;
	releaseCof = calculateCof(sustainLevel, minVal, releaseLength);
}

void ADSr::setSampleRate(double sampleRate)
{
	currentSampleRate = sampleRate;
}


double ADSr::getEnvStage() {
	return env_stage;
}
void ADSr::setEnvStage(EnvelopeStage env)
{
	env_stage = env;
}