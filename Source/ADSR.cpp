

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSR.h"
#include<math.h>
#include <iostream>



ADSr::ADSr()
{

	output = 0;
	env_stage = env_off;
	attackLength = 0.01;
	decayLength = 0.2;
	sustainLevel = 1;
	releaseLength = 0.01;
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
			//output = minVal + attackCof*output;	//Logaritmic
			output = attackCof + output;  //Linear
			if (output > 1)
			{	
				env_stage = env_decay;
				//output = 1;
			}
			break;
		case env_decay:
			output = decayCof*output;
			if (output <= sustainLevel)
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
	attackLength = (minVal+aLength)*5*currentSampleRate;
	attackCof = 1 / attackLength;	//Linear 
	//attackCof = calculateCof(minVal, maxVal, attackLength);	//Logaritmic
	//std::cout << attackCof << std::endl;
}

void ADSr::updateDecay(double dLength) {
	decayLength = (minVal+dLength)*2*currentSampleRate;
	decayCof = calculateCof(maxVal, sustainLevel, decayLength);
	//std::cout << decayCof << std::endl;
}

void ADSr::updateSustain(double sLevel)
{
	sustainLevel = sLevel;
	//std::cout << sustainLevel << std::endl;
}

void ADSr::updateRelease(double rLength) 
{
	releaseLength = (minVal+rLength)*10*currentSampleRate;
	releaseCof = calculateCof(sustainLevel, minVal, releaseLength);
	//std::cout << releaseCof << std::endl;
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