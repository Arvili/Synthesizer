
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class Oscillator    : public Component
{
public:
    Oscillator();
    ~Oscillator();
	
	void setSampleRate(double sampleRate);
	void updateWaveState(int state);
	double returnSample();

	void setFrequency(double freq);
	void updateFrequency();
	void updateDetune(double defreq);
	double getFrequency();

	void setMidiNote(double freq);
	int getMidiNote();



private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
	
	double currentSampleRate = 0.0;
	int waveState = 0;

	Array<float> sineTable;
	Array<float> triangleTable;
	Array<float> trivialSawTable;
	Array<float> bipolarSawTable;
	double wtSize;
	double frequency;
	double phase;
	double increment;
	double prevSawSample;
	double prevSawSample2;
	double phaseShift;
	double sawScale;
	double detune;
	int midiNote;
};
