
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"
#include "Delay.h"
#include "Filter.h"
#include "ADSR.h"
#include <iostream>
#include<cmath>


class MainComponent : public AudioAppComponent,
					  public Slider::Listener,
					  //public Slider::LookAndFeelMethods,
					  //public Button::Listener,
					  public MidiKeyboardStateListener,
						private MidiInputCallback,
					  public KeyListener
					  // DocumentWindow
{
public:
	//MidiKeyboardState keyboardState;
	//MidiKeyboardComponent keyboardComponent;


	MainComponent();
    ~MainComponent();

	

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

	void performLFOMod(int lfoMod, double lfoSample);

	void updateDetune();
	void updateAmplitude();
	void updateWaveform();

	void updateDetune2();
	void updateAmplitude2();
	void updateWaveform2();

	void updateLfoFrequency();
	void updateLfoAmplitude();
	void updateLfoWaveform();
	//void updateLFOTO1();

	void updateFilterFrequency();
	void updateFilterResonance();
	void updateFilterMode();

	void updateA();
	void updateD();
	void updateS();
	void updateR();

	void updateSub();
	void updateSubDetune();
	void updateSubWaveform();
	void updateNoise();
	void updatePanning();

    void paint (Graphics& g) override;
	void DrawADSR(Graphics& g);
    void resized() override;
	void initGUI();

	/*void buttonClicked(Button* button) override {
		if (button == &lfoTo1)
		{
			updateLFOTO1();
		}
		
	};*/

	void sliderValueChanged (Slider* slider) override
	{
		if (slider == &frequencySlider)
		{
			updateDetune();
		}
		if (slider == &amplitudeSlider) 
		{
			updateAmplitude();
		}
		if (slider == &waveSlider)
		{
			updateWaveform();
		}
		if (slider == &frequencySlider2)
		{
			updateDetune2();
		}
		if (slider == &amplitudeSlider2)
		{
			updateAmplitude2();
		}
		if (slider == &waveSlider2)
		{
			updateWaveform2();
		}
		if (slider == &lfoFrequencySlider)
		{
			updateLfoFrequency();
		}
		if (slider == &lfoAmplitudeSlider)
		{
			updateLfoAmplitude();
		}
		if (slider == &lfoWaveSlider)
		{
			updateLfoWaveform();
		}
		if (slider == &filterFrequencySlider)
		{
			updateFilterFrequency();
		}
		if (slider == &filterResonanceSlider)
		{
			updateFilterResonance();
		}
		if (slider == &filterModeSlider)
		{
			updateFilterMode();
		}
		if (slider == &attackSlider)
		{
			updateA();
		}
		if (slider == &decaySlider)
		{
			updateD();
		}
		if (slider == &sustainSlider)
		{
			updateS();
		}
		if (slider == &releaseSlider)
		{
			updateR();
		}
		if (slider == &subSlider)
		{
			updateSub();
		}
		if (slider == &noiseSlider)
		{
			updateNoise();
		}
		if (slider == &subFrequencySlider)
		{
			updateSubDetune();
		}
		if (slider == &subWaveSlider)
		{
			updateSubWaveform();
		}
		if (slider == &panSlider)
		{
			updatePanning();
		}
	};


	bool keyPressed(const KeyPress &key, Component *c) override
	{
		if (key.getTextCharacter() == 'a')
		{
			osc1Freq = 440;
			osc2Freq = 440;
			osc1.setFrequency(440);
			osc2.setFrequency(440);
			sub.setFrequency(55);
			return true;
		}
		if (key.getTextCharacter() == 's')
		{
			osc1Freq = 493.88;
			osc2Freq = 493.88;
			osc1.setFrequency(493.88);
			osc2.setFrequency(493.88);
			sub.setFrequency(61.74);
			return true;
		}
		if (key.getTextCharacter() == 'd')
		{
			osc1Freq = 523.25;
			osc2Freq = 523.25;
			osc1.setFrequency(523.25);
			osc2.setFrequency(523.25);
			sub.setFrequency(65.41);
			return true;
		}
		if (key.getTextCharacter() == 'f')
		{
			osc1Freq = 587.33;
			osc2Freq = 587.33;
			osc1.setFrequency(587.33);
			osc2.setFrequency(587.33);
			sub.setFrequency(73.42);
			return true;
		}
		if (key.getTextCharacter() == 'g')
		{
			osc1Freq = 659.25;
			osc2Freq = 659.25;
			osc1.setFrequency(659.25);
			osc2.setFrequency(659.25);
			sub.setFrequency(82.41);
			return true;
		}
		if (key.getTextCharacter() == 'h')
		{
			osc1Freq = 698.46;
			osc2Freq = 698.46;
			osc1.setFrequency(698.46);
			osc2.setFrequency(698.46);
			sub.setFrequency(87.31);
			return true;
		}
		if (key.getTextCharacter() == 'j')
		{
			osc1Freq = 783.99;
			osc2Freq = 783.99;
			osc1.setFrequency(783.99);
			osc2.setFrequency(783.99);
			sub.setFrequency(98);
			return true;
		}
		if (key.getTextCharacter() == 'k')
		{
			osc1Freq = 880;
			osc2Freq = 880;
			osc1.setFrequency(880.00);
			osc2.setFrequency(880.00);
			sub.setFrequency(110);
			return true;
		}
	};

	bool keyStateChanged(bool isKeyDown, Component *c)
	{
		if (isKeyDown == true)
		{
			ADSR.setEnvStage(ADSr::env_attack);
		}
		if (isKeyDown == false)
		{
			ADSR.setEnvStage(ADSr::env_release);
		}
		return isKeyDown;
	}

	void filterMenuChanged()
	{
		updateFilterMode();
	}

	/*void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{


	}
	*/
	void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
	{
		if (!isAddingFromMidiInput)
		{
			auto m = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
			m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
			
			//midiEvents.addEvent(m, );
		}
		//std::cout << Time::getMillisecondCounterHiRes() << std::endl;
		double freq = pow(2.0, ((midiNoteNumber - 69.0) / 12.0) ) * 440;
		double subfreq = pow(2.0, ((midiNoteNumber - 24.0 - 69.0) / 12.0)) * 440;
		osc1.setFrequency(freq);
		updateDetune();
		osc2.setFrequency(freq);
		updateDetune2();
		sub.setFrequency(subfreq);
		updateSubDetune();
		ADSR.setEnvStage(ADSr::env_attack);

		

		
		
	};

	void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
	{
		if (!isAddingFromMidiInput)
		{
			auto m = MidiMessage::noteOff(midiChannel, midiNoteNumber);
			m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
			
		}
		
		if (midiNoteNumber == osc1.getMidiNote())
		{
			ADSR.setEnvStage(ADSr::env_release);
		}
		
	};
	

	void populateMidiList();
	void setMidiInput(int index);

	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
	
private:
	Label oscillator1_label;
	Label oscillator2_label;
	Label lfo_label;
	Label filter_label;
	Label sub_label;
	Label noise_label;
	Label pan_label;

	Slider frequencySlider;
	Slider amplitudeSlider;
	Slider waveSlider;

	Slider filterFrequencySlider;
	Slider filterResonanceSlider;
	Slider filterModeSlider;
	ComboBox filterMenu;

	Slider frequencySlider2;
	Slider amplitudeSlider2;
	Slider waveSlider2;

	Slider lfoFrequencySlider;
	Slider lfoAmplitudeSlider;
	Slider lfoWaveSlider;
	ComboBox lfoMenu;

	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;

	Slider subSlider;
	Slider subFrequencySlider;
	Slider subWaveSlider;

	Slider noiseSlider;
	Slider panSlider;

	TextButton lfoTo1;
	TextButton lfoTo2;

	double currentSampleRate = 0.0;
	double level = 0;
	int waveState = 0;

	double level2 = 0;
	int waveState2 = 0;

	double levelSub = 0;
	double levelNoise = 0;

	double levelLfo = 0;
	int waveStateLfo = 0;

	double ADSRCof = 0;
	double filterCutoff;

	double rPan = 1;
	double lPan = 1;

	Oscillator osc1;
	Oscillator osc2;
	Oscillator lfo;
	Oscillator sub;

	Delay delay;
	Filter filter;
	ADSr ADSR;

	bool isNotMuted = false;
	Point<int> ADSRStart;

	double osc1Freq;
	double osc2Freq;


	AudioDeviceManager deviceManager;           
	ComboBox midiInputList;                    
	Label midiInputListLabel;
	int lastInputIndex = 0;                           
	bool isAddingFromMidiInput = false;              

	MidiKeyboardState keyboardState;           
	MidiKeyboardComponent keyboardComponent;   

	Array<int> midiEvents[127];
	int midiSample = 0;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
