
#include "MainComponent.h"
#include <math.h>
#include "Oscillator.h"
#include <random>
#include <ctime>
#include <cstdlib>


MainComponent::MainComponent()
	: keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
	setSize(1000, 500);		//Size of window
	initGUI();				//Initialize GUI

	/*auto midiInputs = MidiInput::getDevices();
	for (auto midiInput : midiInputs)
	{
		if (deviceManager.isMidiInputEnabled(midiInput))
		{
			keyboardComponent.setMidiInput(midiInputs.indexOf(midiInput));
			break;
		}
	}*/

	setAudioChannels(0, 2);	//Set 2 output channels, left and right

	keyboardState.addListener(this);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}



void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	/*Called before any samples are delivered to device, usually when the program starts*/

	amplitudeSlider.setValue(0.5);			//Oscillator 1 amplitude value is initialized to 0.5 dB
	waveSlider.setValue(0);					//Oscillator 1 waveform is set to sin-wave
	frequencySlider.setValue(0);			//Oscillator 1 detune is set to 0 Hz
	filterFrequencySlider.setValue(0.99);	//Filter cutoff frequency is set to 20 kHz
	filterCutoff = 0.99;

	amplitudeSlider2.setValue(0);			//Oscillator 2 amplitude is initialized to 0 db
	waveSlider2.setValue(0);				//Oscillator 2 waveform is set to sin-wave
	frequencySlider2.setValue(0);			//Oscillator 2 detune is set to 0 Hz

	lfo.setFrequency(lfoFrequencySlider.getValue());
	lfoAmplitudeSlider.setValue(0.5);		//LFO ampltidue is set to 0.5 dB

	/*ADSR sliders positions are set*/
	attackSlider.setValue(0.01);				
	decaySlider.setValue(1);				
	sustainSlider.setValue(1);			
	releaseSlider.setValue(0.01);		

	subSlider.setValue(0);					//Suboscillator amplitude is set to 0
	panSlider.setValue(0);

	level = amplitudeSlider.getValue();		//Set level coeffisient

	/*Set samplerates*/
	osc1.setSampleRate(sampleRate);			
	osc2.setSampleRate(sampleRate);
	lfo.setSampleRate(sampleRate);
	sub.setSampleRate(sampleRate);
	ADSR.setSampleRate(sampleRate);


	srand(static_cast <unsigned> (time(0)));		//Initialize srand with random variable

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);	//Audio buffers to sounddevice
	auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	double currentSample = 0;	//Init current sample as 0
	double lfoSample = 0;		//Lfo controls through its own samples
	int lfoMod = lfoMenu.getSelectedId();

	/*Iterate over the size of audiobuffers*/
	for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
		ADSRCof = ADSR.Process();	//ADSR coeffisient, affects to overal amplitude

		double lfoSample = levelLfo * lfo.returnSample();	//Lfo sample, differents between -1...1
		if (lfoMod != 0) { performLFOMod(lfoMod, lfoSample); }
		/*Calculate current sample*/
		currentSample = ADSRCof*(level * osc1.returnSample() +		//Oscillator 1
						level2 *  osc2.returnSample() +	//Oscillator 2
						levelSub*sub.returnSample() +		//Sub-oscillator
			levelNoise * ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5));	//Noise

		currentSample = filter.process(currentSample);	//Filter sample
		leftBuffer[sample] = lPan*currentSample;		//Fill audiobuffers with samples
		rightBuffer[sample] = rPan*currentSample;
	}
	/*if (delay.ison() == true)
	{
		delay.adddelay(rightbuffer, leftbuffer);
	}
	*/
	osc1.updateFrequency();
	
}

void MainComponent::performLFOMod(int lfoMod, double lfoSample)
{
	switch (lfoMod) {	//switch/case between features to affect with lfo
		case 2:
			osc1.setFrequency(osc1Freq + 10 * lfoSample);	//Lfo modulates the frequency of osc1 (vibrato)
			break;
		case 3:
			osc2.setFrequency(osc2Freq + 10 * lfoSample); //Lfo modulates the frequency of osc2 (vibrato)
			break;
		case 4:
			//filterFrequencySlider.setValue(abs(levelLfo * lfo.returnSample()));
			filter.setCutoff(filterCutoff + lfoSample);	//Lfo modulates filter cutoff
	}
}


void MainComponent::updateDetune()
{
	/*Update oscillator 1 detune*/
	osc1.updateDetune((double)frequencySlider.getValue());
}

void MainComponent::updateAmplitude()
{
	/*Update oscillator 1 amplitude*/
	level = amplitudeSlider.getValue();
}

void MainComponent::updateWaveform()
{
	/*Change waveform of oscillator 1*/
	osc1.updateWaveState((int)waveSlider.getValue());
}

void MainComponent::updateDetune2()
{
	/*Update oscillator 2 detune*/
	osc2.updateDetune((double)frequencySlider2.getValue());

}

void MainComponent::updateAmplitude2()
{
	/*update oscillator 2 amplitude*/
	level2 = amplitudeSlider2.getValue();
}

void MainComponent::updateWaveform2()
{
	/*Change waveform of oscillator 2*/
	osc2.updateWaveState(waveSlider2.getValue());
}

void MainComponent::updateLfoFrequency()
{
	/*Update LFO frequency*/
	lfo.setFrequency(lfoFrequencySlider.getValue());
}

void MainComponent::updateLfoAmplitude()
{
	/*Update LFO amplitude*/
	levelLfo = lfoAmplitudeSlider.getValue();
}

void MainComponent::updateLfoWaveform()
{
	/*Change waveform of LFO*/
	lfo.updateWaveState(lfoWaveSlider.getValue());
}


void MainComponent::updateFilterFrequency()
{
	/*Update filter cutoff frequency*/
	filter.setCutoff((double)filterFrequencySlider.getValue());
	filterCutoff = (double)filterFrequencySlider.getValue();
}

void MainComponent::updateFilterResonance()
{
	/*Update filter resonance*/
	filter.setResonance((double)filterResonanceSlider.getValue());
}

void MainComponent::updateFilterMode()
{
	/*Update mode of filter*/
	//filter.setMode((int)filterModeSlider.getValue());
	filter.setMode(filterMenu.getSelectedId());
}

void MainComponent::updateA() 
{
	/*Update length of attack*/
	ADSR.updateAttack(attackSlider.getValue());
}
void MainComponent::updateD()
{
	/*Update length of decay*/
	ADSR.updateDecay(decaySlider.getValue());
}
void MainComponent::updateS()
{
	/*Update level of sustain*/
	ADSR.updateSustain(sustainSlider.getValue());
}
void MainComponent::updateR()
{
	/*Update length of release*/
	ADSR.updateRelease(releaseSlider.getValue());
}

void MainComponent::updateSub()
{
	/*Update sub oscillator amplitude*/
	levelSub = subSlider.getValue();
}

void MainComponent::updateSubDetune()
{
	/*Update sub detune*/
	sub.updateDetune((double)subFrequencySlider.getValue());
}

void MainComponent::updateSubWaveform()
{
	/*Update sub waveform*/
	sub.updateWaveState(subWaveSlider.getValue());
}

void MainComponent::updateNoise()
{
	/*Update amplitude of noise*/
	levelNoise = noiseSlider.getValue();
}

void MainComponent::updatePanning()
{
	/*Pan to left or rigth*/
	double panning = panSlider.getValue();	//Value of pan
	if (panning >= 0)
	{
		lPan = 1-panning;
		rPan = 1;
	}
	else
	{
		lPan = 1;
		rPan = 1-abs(panning);
	}
}

void MainComponent::populateMidiList()
{
	midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
	auto midiInputs = juce::MidiInput::getAvailableDevices();

	StringArray midiInputNames;

	for (auto input : midiInputs)
		midiInputNames.add(input.name);

	midiInputList.addItemList(midiInputNames, 1);
	midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };

	// find the first enabled device and use that by default
	for (auto input : midiInputs)
	{
		if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
		{
			setMidiInput(midiInputs.indexOf(input));
			break;
		}
	}

	// if no enabled devices were found just use the first one in the list
	if (midiInputList.getSelectedId() == 0)
		setMidiInput(0);
}

void MainComponent::setMidiInput(int index)
{
	auto list = MidiInput::getAvailableDevices();

	deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);

	auto newInput = list[index];

	if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
		deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

	deviceManager.addMidiInputDeviceCallback(newInput.identifier, this);
	midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

	lastInputIndex = index;
}


void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
	const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
	keyboardState.processNextMidiEvent(message);
	//postMessageToList(message, source->getName());
}


void MainComponent::releaseResources()
{
   
}


void MainComponent::paint (Graphics& g)
{
	/*Paint GUI*/

	/*
	Background: 0xff333333
	SliderBackgrond: 0xff444444
	Oscillator: 0xff00805e
	Lfo: 0xff808000
	Filter: 0xff800080
	ADSR: 0xff800000
	Sub: 0xff002080
	Noise: 0xffd9d9d9
	*/

	g.fillAll(Colour(0xff333333));

	
	getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colour(0xff555555));
	getLookAndFeel().setColour(Slider::backgroundColourId, Colour(0xff555555));
	getLookAndFeel().setColour(Slider::thumbColourId, Colour(0xff444444));
	getLookAndFeel().setColour(ComboBox::backgroundColourId, Colour(0xff555555));
	getLookAndFeel().setColour(ComboBox::arrowColourId, Colour(0xff555555));
	getLookAndFeel().setColour(ComboBox::buttonColourId, Colour(0xff555555));
	
	frequencySlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff00805e));
	amplitudeSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff00805e));
	waveSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff00805e));

	frequencySlider2.setColour(Slider::rotarySliderFillColourId, Colour(0xff00805e));
	amplitudeSlider2.setColour(Slider::rotarySliderFillColourId, Colour(0xff00805e));
	waveSlider2.setColour(Slider::rotarySliderFillColourId, Colour(0xff00805e));
	

	lfoFrequencySlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff808000));
	lfoAmplitudeSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff808000));
	lfoWaveSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff808000));
	
	filterFrequencySlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff800080));
	filterResonanceSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff800080));

	attackSlider.setColour(Slider::trackColourId, Colour(0xff800000));
	decaySlider.setColour(Slider::trackColourId, Colour(0xff800000));
	sustainSlider.setColour(Slider::trackColourId, Colour(0xff800000));
	releaseSlider.setColour(Slider::trackColourId, Colour(0xff800000));

	subSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff002080));
	subFrequencySlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff002080));
	subWaveSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xff002080));
	noiseSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xffd9d9d9));
		//frequencySlider.setLookAndFeel();
	int itemMargin = 10;
	Rectangle<int> area = getLocalBounds();
	Rectangle<int> control = area.removeFromTop(area.getHeight()/4).reduced(itemMargin);
	Rectangle<int> controlArea = area;
	Rectangle<int> oscillatorArea = controlArea.removeFromLeft(3*(controlArea.getWidth() / 7));
	Rectangle<int> osc1Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	Rectangle<int> osc2Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	Rectangle<int> subArea = oscillatorArea.reduced(itemMargin);
	Rectangle<int> filterArea = (controlArea.removeFromLeft(controlArea.getWidth() / 4)).reduced(itemMargin);
	Rectangle<int> ADSRArea = controlArea.removeFromBottom(controlArea.getHeight()/3).reduced(itemMargin);
	Rectangle<int> lfoArea = controlArea.removeFromTop(controlArea.getHeight()/2).reduced(itemMargin);
	Rectangle<int> tweekArea = controlArea.reduced(itemMargin);
	//Rectangle<int> noiseArea = controlArea.reduced(itemMargin);
	//Rectangle<int> plotPlot = plotArea.reduced(itemMargin);
	g.setColour(Colour(0xff444444));
	g.fillRoundedRectangle(control.toFloat(), 10.0f);
	g.fillRoundedRectangle(osc1Area.toFloat(), 10.0f);
	g.fillRoundedRectangle(osc2Area.toFloat(), 10.0f);
	g.fillRoundedRectangle(lfoArea.toFloat(), 10.0f);
	g.fillRoundedRectangle(filterArea.toFloat(), 10.0f);
	g.fillRoundedRectangle(ADSRArea.toFloat(), 10.0f);
	g.fillRoundedRectangle(subArea.toFloat(), 10.0f);
	g.fillRoundedRectangle(tweekArea.toFloat(), 10.0f);
	//g.fillRect(noiseArea);
	//g.setColour(Colour(0xff555555));
	//g.fillRect(plotArea.reduced(itemMargin));
	//g.fillRect(plotPlot);
	//ADSRStart = plotPlot.getBottomLeft();
	//DrawADSR(g);

}

void MainComponent::DrawADSR(Graphics& g) {
	/*Path path;
	path.startNewSubPath(645,180);
	path.lineTo(750, 100);
	g.setColour(Colour(0xff800000));
	g.fillPath(path);
	*/
}
  

void MainComponent::resized()
{
	int itemMargin = 15;

	Rectangle<int> area = getLocalBounds();
	Rectangle<int> control = area.removeFromTop(area.getHeight() / 4).reduced(itemMargin);
	Rectangle<int> panArea = control.removeFromRight(control.getWidth() / 5);
	Rectangle<int> midiInputArea = control.removeFromLeft(control.getWidth() / 4);

	Rectangle<int> controlArea = area;
	Rectangle<int> oscillatorArea = controlArea.removeFromLeft(3*(controlArea.getWidth() / 7));
	Rectangle<int> osc1Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	
	
	Rectangle<int> osc2Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	Rectangle<int> subArea = oscillatorArea.reduced(itemMargin);
	Rectangle<int> subLabel = subArea.removeFromLeft(oscillatorArea.getWidth() / 6);
	//Rectangle<int> keyboardArea = area.removeFromBottom(area.getHeight() / 3);
	Rectangle<int> filterArea = (controlArea.removeFromLeft(controlArea.getWidth() / 4)).reduced(itemMargin);
	Rectangle<int> ADSRControlArea = controlArea.removeFromBottom(controlArea.getHeight()/3).reduced(itemMargin); //.removeFromBottom(controlArea.getHeight() / 2);
	Rectangle<int> lfoArea = controlArea.removeFromTop(controlArea.getHeight() / 2).reduced(itemMargin);
	Rectangle<int> lfoLabel = lfoArea.removeFromLeft(oscillatorArea.getWidth() / 6);
	Rectangle<int> tweekArea = controlArea.reduced(itemMargin);

	oscillator1_label.setBounds(osc1Area.removeFromLeft(oscillatorArea.getWidth() / 6));
	amplitudeSlider.setBounds(osc1Area.removeFromLeft(osc1Area.getWidth() / 3));
	frequencySlider.setBounds(osc1Area.removeFromLeft(osc1Area.getWidth() / 2));
	waveSlider.setBounds(osc1Area);

	oscillator2_label.setBounds(osc2Area.removeFromLeft(oscillatorArea.getWidth() / 6));
	amplitudeSlider2.setBounds(osc2Area.removeFromLeft(osc2Area.getWidth() / 3));
	frequencySlider2.setBounds(osc2Area.removeFromLeft(osc2Area.getWidth() / 2));
	waveSlider2.setBounds(osc2Area);

	lfo_label.setBounds(lfoLabel.removeFromTop(lfoLabel.getHeight()/2));
	lfoMenu.setBounds(lfoLabel.getX()+5, lfoLabel.getY(), lfoLabel.getWidth()-10, lfoLabel.getHeight()-20);
	//lfoTo1.setBounds(lfoLabel.removeFromTop(lfoLabel.getHeight() / 2));
	//lfoTo2.setBounds(lfoLabel);
	lfoAmplitudeSlider.setBounds(lfoArea.removeFromLeft(lfoArea.getWidth() / 3));
	lfoFrequencySlider.setBounds(lfoArea.removeFromLeft(lfoArea.getWidth() / 2));
	lfoWaveSlider.setBounds(lfoArea);

	//keyboardComponent.setBounds(keyboardArea);

	filter_label.setBounds(filterArea.removeFromTop(filterArea.getHeight() / 6));
	filterFrequencySlider.setBounds(filterArea.removeFromTop(filterArea.getHeight() / 3));
	filterResonanceSlider.setBounds(filterArea.removeFromTop(filterArea.getHeight() / 2));
	//filterModeSlider.setBounds(filterArea);
	filterMenu.setBounds(filterArea.getX()+5,filterArea.getY()+30,filterArea.getWidth()-10, filterArea.getHeight()-60);

	/*
	attackSlider.setBounds(ADSRArea.getX(), ADSRArea.getY()+ADSRArea.getHeight()/4, ADSRArea.getWidth()/4, ADSRArea.getHeight()/2);
	decaySlider.setBounds(ADSRArea.getX()+ADSRArea.getWidth()/4, ADSRArea.getY() + ADSRArea.getHeight() / 4, ADSRArea.getWidth() / 4, ADSRArea.getHeight() / 2);
	sustainSlider.setBounds(ADSRArea.getX() + 2*ADSRArea.getWidth() / 4, ADSRArea.getY() + ADSRArea.getHeight() / 4, ADSRArea.getWidth() / 4, ADSRArea.getHeight() / 2);
	releaseSlider.setBounds(ADSRArea.getX() + 3*ADSRArea.getWidth() / 4, ADSRArea.getY() + ADSRArea.getHeight() / 4, ADSRArea.getWidth() / 4, ADSRArea.getHeight() / 2);
	*/
	attackSlider.setBounds(ADSRControlArea.removeFromLeft(ADSRControlArea.getWidth() / 4));
	decaySlider.setBounds(ADSRControlArea.removeFromLeft(ADSRControlArea.getWidth() / 3));
	sustainSlider.setBounds(ADSRControlArea.removeFromLeft(ADSRControlArea.getWidth() / 2));
	releaseSlider.setBounds(ADSRControlArea.removeFromLeft(ADSRControlArea.getWidth()));

	sub_label.setBounds(subLabel);
	subSlider.setBounds(subArea.removeFromLeft(subArea.getWidth()/3));
	subFrequencySlider.setBounds(subArea.removeFromLeft(subArea.getWidth() / 2));
	subWaveSlider.setBounds(subArea.removeFromLeft(subArea.getWidth()));
	noise_label.setBounds(tweekArea.removeFromLeft(tweekArea.getWidth() / 9));
	noiseSlider.setBounds(tweekArea.removeFromLeft(tweekArea.getWidth()/4));

	pan_label.setBounds(panArea.removeFromLeft(panArea.getHeight()/3));
	panSlider.setBounds(panArea);

	midiInputList.setBounds(midiInputArea);
}

void MainComponent::initGUI()
{
	
	/*Initialize and setup every slider and label*/

	/*Keyboud focus and listener to listen keyboard commands*/
	setWantsKeyboardFocus(true);	
	addKeyListener(this);
	grabKeyboardFocus();
	

	//addAndMakeVisible(keyboardComponent);
	//keyboardState.addListener(this);

	/*Init labels*/
	addAndMakeVisible(oscillator1_label);
	oscillator1_label.setText("Osc I", NotificationType::dontSendNotification);
	addAndMakeVisible(oscillator2_label);
	oscillator2_label.setText("Osc II", NotificationType::dontSendNotification);
	addAndMakeVisible(lfo_label);
	lfo_label.setText("LFO", NotificationType::dontSendNotification);
	addAndMakeVisible(filter_label);
	filter_label.setText("Filter", NotificationType::dontSendNotification);
	addAndMakeVisible(sub_label);
	sub_label.setText("Sub", NotificationType::dontSendNotification);
	addAndMakeVisible(noise_label);
	noise_label.setText("Noise", NotificationType::dontSendNotification);
	//addAndMakeVisible(pan_label);
	//pan_label.setText("Pan L/R", NotificationType::dontSendNotification);
	

	/*Initi sliders, for every slider almost identical setup*/

	addAndMakeVisible(frequencySlider);										//add and make visible on GUI
	frequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);			//Set slider style, usually rotary, in adsr vertical
	frequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);	//Set text box below the slider
	frequencySlider.setRange(-1, 1, 0.01);									//Set range of slider							
	frequencySlider.addListener(this);										//Add listener to listen changes of slider values
	

	addAndMakeVisible(amplitudeSlider);
	amplitudeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	amplitudeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	amplitudeSlider.setRange(0, 1, 0.01);
	amplitudeSlider.setTextValueSuffix(" dB");								//Set suffix of slider values
	amplitudeSlider.addListener(this);

	addAndMakeVisible(waveSlider);
	waveSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	waveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	waveSlider.setRange(0, 3, 1);
	waveSlider.addListener(this);

	addAndMakeVisible(frequencySlider2);
	frequencySlider2.setSliderStyle(Slider::SliderStyle::Rotary);
	frequencySlider2.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	frequencySlider2.setRange(-1, 1, 0.01);
	frequencySlider2.addListener(this);

	addAndMakeVisible(amplitudeSlider2);
	amplitudeSlider2.setSliderStyle(Slider::SliderStyle::Rotary);
	amplitudeSlider2.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	amplitudeSlider2.setRange(0, 1, 0.01);
	amplitudeSlider2.setTextValueSuffix(" dB");
	amplitudeSlider2.addListener(this);

	addAndMakeVisible(waveSlider2);
	waveSlider2.setSliderStyle(Slider::SliderStyle::Rotary);
	waveSlider2.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	waveSlider2.setRange(0, 3, 1);
	waveSlider2.addListener(this);

	addAndMakeVisible(lfoFrequencySlider);
	lfoFrequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	lfoFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	lfoFrequencySlider.setRange(1, 20, 1);
	lfoFrequencySlider.setTextValueSuffix(" Hz");
	lfoFrequencySlider.addListener(this);

	addAndMakeVisible(lfoAmplitudeSlider);
	lfoAmplitudeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	lfoAmplitudeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	lfoAmplitudeSlider.setRange(0, 1, 0.01);
	lfoAmplitudeSlider.setTextValueSuffix(" Depth");
	lfoAmplitudeSlider.addListener(this);

	addAndMakeVisible(lfoWaveSlider);
	lfoWaveSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	lfoWaveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	lfoWaveSlider.setRange(0, 3, 1);
	lfoWaveSlider.addListener(this);

	addAndMakeVisible(lfoMenu);
	lfoMenu.addItem("-", 1);
	lfoMenu.addItem("Osc I", 2);
	lfoMenu.addItem("Osc II", 3);
	lfoMenu.addItem("Filter cutoff", 4);
	lfoMenu.setSelectedId(1, false);

	/*addAndMakeVisible(lfoTo1);
	lfoTo1.setToggleState(false, NotificationType::dontSendNotification);
	lfoTo1.setClickingTogglesState(true);
	lfoTo1.addListener(this);

	addAndMakeVisible(lfoTo2);
	lfoTo2.setToggleState(false, NotificationType::dontSendNotification);
	lfoTo2.setClickingTogglesState(true);*/

	addAndMakeVisible(filterFrequencySlider);
	filterFrequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	filterFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	filterFrequencySlider.setRange(0, 0.99, 0.01);
	//filterFrequencySlider.setTextValueSuffix(" Hz");
	filterFrequencySlider.addListener(this);

	addAndMakeVisible(filterResonanceSlider);
	filterResonanceSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	filterResonanceSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	filterResonanceSlider.setRange(0, 1, 0.01);
	//filterResonanceSlider.setTextValueSuffix(" Hz");
	filterResonanceSlider.addListener(this);

	addAndMakeVisible(filterModeSlider);
	filterModeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	filterModeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	filterModeSlider.setRange(1, 3, 1);
	//filterModeSlider.setTextValueSuffix(" mode");
	filterModeSlider.addListener(this);

	addAndMakeVisible(filterMenu);
	filterMenu.addItem("Lowpass", 1);
	filterMenu.addItem("Highpass", 2);
	filterMenu.addItem("Bandpass", 3);
	filterMenu.onChange = [this] { filterMenuChanged(); };
	filterMenu.setSelectedId(1);


	addAndMakeVisible(attackSlider);
	attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	attackSlider.setRange(0.01, 1, 0.01);
	attackSlider.addListener(this);


	addAndMakeVisible(decaySlider);
	decaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	decaySlider.setRange(0.01, 1, 0.01);
	decaySlider.addListener(this);

	addAndMakeVisible(sustainSlider);
	sustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	sustainSlider.setRange(0.01, 1, 0.01);
	sustainSlider.addListener(this);


	addAndMakeVisible(releaseSlider);
	releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	releaseSlider.setRange(0.01, 1, 0.01);
	releaseSlider.addListener(this);

	addAndMakeVisible(subSlider);
	subSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	subSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	subSlider.setRange(0, 1, 0.01);
	subSlider.setTextValueSuffix(" dB");
	subSlider.addListener(this);

	addAndMakeVisible(subWaveSlider);
	subWaveSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	subWaveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	subWaveSlider.setRange(0, 3, 1);
	subWaveSlider.addListener(this);

	addAndMakeVisible(subFrequencySlider);
	subFrequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	subFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	subFrequencySlider.setRange(-1, 1, 0.01);
	subFrequencySlider.addListener(this);

	addAndMakeVisible(noiseSlider);
	noiseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	noiseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	noiseSlider.setRange(0, 1, 0.01);
	noiseSlider.setTextValueSuffix(" dB");
	noiseSlider.addListener(this);

	addAndMakeVisible(panSlider);
	panSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	panSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	panSlider.setRange(-1, 1, 0.01);
	//panSlider.setTextValueSuffix();
	panSlider.addListener(this);

	addAndMakeVisible(midiInputList);
	populateMidiList();
}
