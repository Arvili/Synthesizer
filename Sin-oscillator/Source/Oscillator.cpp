
#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"


 Oscillator::Oscillator()
{
	
	frequency = 440;
	phase = 0;
	wtSize = 1024;
	double s = -1;
	prevSawSample = 0;
	prevSawSample2 = 0;
	phaseShift = wtSize/2;
	detune = 0;
	

	for (int i = 0; i < wtSize; i++)
	{
		sineTable.insert(i, std::sin(2 * double_Pi * i / wtSize));
	}

	for (int j = 0; j < wtSize; j++)
	{
		triangleTable.insert(j, (float)(acos(std::sin(2 * double_Pi * j / wtSize)) / 1.5708));
		//triangleTable.insert(j, (float)abs(fmod(((j/wtSize) + double_Pi) / (double_Pi), 2)-1));
	}

	for (int k = 0; k < wtSize; k++)
	{

		trivialSawTable.insert(k, s);
		s += 1 / (wtSize / 2);
	}

}

Oscillator::~Oscillator()
{
}

void Oscillator::setSampleRate(double sampleRate)
{
	currentSampleRate = sampleRate;
	increment = frequency * wtSize / currentSampleRate;
	sawScale = currentSampleRate / (4 * frequency);
}

void Oscillator::updateWaveState(int state)
{
	waveState = state;
}

double Oscillator::returnSample()
{
	float sample = 0;
	float shiftedSample = 0;
	switch (waveState) {
	case 0:
		sample = sineTable[(int)phase];

		break;
	case 1:
		sample = triangleTable[(int)phase];
		break;
	case 2:
		
		sample = sawScale * (pow(trivialSawTable[(int)phase], 2) - pow(prevSawSample, 2));
		prevSawSample = trivialSawTable[(int)phase];
		break;
	case 3:
		sample = sawScale * (pow(trivialSawTable[(int)phase], 2) - pow(prevSawSample, 2));
		prevSawSample = trivialSawTable[(int)phase];
		shiftedSample = sawScale * (pow(trivialSawTable[(int)phaseShift], 2) - pow(prevSawSample2, 2));
		prevSawSample2 = trivialSawTable[(int)phaseShift];
		sample = sample - shiftedSample;
		break;
	}
	phase = fmod(phase + increment, wtSize);
	phaseShift = fmod(phaseShift + increment, wtSize);
	return sample;
}

void Oscillator::updateFrequency(double freq)
{
	frequency = freq + detune;
	increment = frequency * wtSize / currentSampleRate;
	sawScale = currentSampleRate / (4 * frequency);
}

void Oscillator::updateDetune(double defreq)
{
	detune = defreq;
}

double Oscillator::getFrequency()
{
	return frequency;
}