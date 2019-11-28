
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"
#include "Delay.h"
#include "Filter.h"
#include "ADSR.h"
#include <iostream>

class MainComponent : public AudioAppComponent,
					  public Slider::Listener,
					  //public Slider::LookAndFeelMethods,
					  //public Button::Listener,
					  //public MidiKeyboardStateListener,
					  public KeyListener
{
public:
	MidiKeyboardState keyboardState;
	MidiKeyboardComponent keyboardComponent;


	MainComponent();
    ~MainComponent();

	

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

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

    void paint (Graphics& g) override;
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
			std::cout << "updateA" << std::endl;
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
	};


	bool keyPressed(const KeyPress &key, Component *c) override
	{
		if (key.getTextCharacter() == 'a')
		{
			osc1.updateFrequency(440);
			osc2.updateFrequency(440);
			return true;
		}
		if (key.getTextCharacter() == 's')
		{
			osc1.updateFrequency(493.88);
			osc2.updateFrequency(493.88);
			return true;
		}
		if (key.getTextCharacter() == 'd')
		{
			osc1.updateFrequency(523.25);
			osc2.updateFrequency(523.25);
			return true;
		}
		if (key.getTextCharacter() == 'f')
		{
			osc1.updateFrequency(587.33);
			osc2.updateFrequency(587.33);
			return true;
		}
		if (key.getTextCharacter() == 'g')
		{
			osc1.updateFrequency(659.25);
			osc2.updateFrequency(659.25);
			return true;
		}
		if (key.getTextCharacter() == 'h')
		{
			osc1.updateFrequency(698.46);
			osc2.updateFrequency(698.46);
			return true;
		}
		if (key.getTextCharacter() == 'j')
		{
			osc1.updateFrequency(783.99);
			osc2.updateFrequency(783.99);
			return true;
		}
		if (key.getTextCharacter() == 'k')
		{
			osc1.updateFrequency(880.00);
			osc2.updateFrequency(880.00);
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
	/*void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
	{
		if (!isAddingFromMidiInput)
		{
			auto m = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
			m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
			postMessageToList(m, "On-Screen Keyboard");
		}
	};
	void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, /*float velocity*///)
	/* override
	{
		if (!isAddingFromMidiInput)
		{
			auto m = MidiMessage::noteOff(midiChannel, midiNoteNumber);
			m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
			postMessageToList(m, "On-Screen Keyboard");
		}
	};*/
	


private:
	Label oscillator1_label;
	Label oscillator2_label;
	Label lfo_label;
	Label filter_label;

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

	TextButton lfoTo1;
	TextButton lfoTo2;

	double currentSampleRate = 0.0;
	double level = 0;
	int waveState = 0;

	double level2 = 0;
	int waveState2 = 0;

	double levelLfo = 0;
	int waveStateLfo = 0;

	double ADSRCof = 0;

	Oscillator osc1;
	Oscillator osc2;
	Oscillator lfo;

	Delay delay;
	Filter filter;
	ADSr ADSR;

	bool isNotMuted = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
