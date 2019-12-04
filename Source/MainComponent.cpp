/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include <math.h>
#include "Oscillator.h"

MainComponent::MainComponent()
	: keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
	setSize(1000, 400);
	initGUI();
	/*auto midiInputs = MidiInput::getDevices();
	for (auto midiInput : midiInputs)
	{
		if (deviceManager.isMidiInputEnabled(midiInput))
		{
			keyboardComponent.setMidiInput(midiInputs.indexOf(midiInput));
			break;
		}
	}*/
	setAudioChannels(0, 2);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}



void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

	amplitudeSlider.setValue(0.5);
	waveSlider.setValue(0);
	frequencySlider.setValue(0);
	filterFrequencySlider.setValue(20000);

	amplitudeSlider2.setValue(0);
	waveSlider2.setValue(0);
	frequencySlider2.setValue(0);

	lfoAmplitudeSlider.setValue(0.5);

	attackSlider.setValue(0.5);
	decaySlider.setValue(0.5);
	sustainSlider.setValue(0.8);
	releaseSlider.setValue(0.5);
	
	level = amplitudeSlider.getValue();
	osc1.setSampleRate(sampleRate);
	osc2.setSampleRate(sampleRate);
	lfo.setSampleRate(sampleRate);
	ADSR.setSampleRate(sampleRate);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	float currentSample = 0;
	if (lfoMenu.getSelectedId() == 4)
	{
		filterFrequencySlider.setValue(levelLfo * lfo.returnSample());
	}
	for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
		ADSRCof = ADSR.Process();
		currentSample = level * ADSRCof * osc1.returnSample() + level2 * ADSRCof * osc2.returnSample();
		currentSample = filter.process(currentSample);
		leftBuffer[sample] = currentSample;
		rightBuffer[sample] = currentSample;
	}
	if (delay.isOn() == true)
	{
		delay.addDelay(rightBuffer, leftBuffer);
	}
	
}


void MainComponent::updateDetune()
{

	osc1.updateDetune((double)frequencySlider.getValue());
	
}

void MainComponent::updateAmplitude()
{
	level = amplitudeSlider.getValue();
}

void MainComponent::updateWaveform()
{
	osc1.updateWaveState(waveSlider.getValue());
}

void MainComponent::updateDetune2()
{

	osc2.updateDetune((double)frequencySlider2.getValue());

}

void MainComponent::updateAmplitude2()
{
	level2 = amplitudeSlider2.getValue();
}

void MainComponent::updateWaveform2()
{
	osc2.updateWaveState(waveSlider2.getValue());
}

void MainComponent::updateLfoFrequency()
{
	lfo.updateFrequency(lfoFrequencySlider.getValue());
}

void MainComponent::updateLfoAmplitude()
{
	levelLfo = lfoAmplitudeSlider.getValue();
}

void MainComponent::updateLfoWaveform()
{
	lfo.updateWaveState(lfoWaveSlider.getValue());
}
/*void MainComponent::updateLFOTO1()
{
	if (lfoTo1.getToggleState() == true)
	{
		lfoTo1.setToggleState(false, NotificationType::dontSendNotification);
	}
	else
	{

		lfoTo1.setToggleState(true, NotificationType::dontSendNotification);
	}
}*/

void MainComponent::updateFilterFrequency()
{
	filter.setCutoff((double)filterFrequencySlider.getValue());
}

void MainComponent::updateFilterResonance()
{
	filter.setResonance((double)filterResonanceSlider.getValue());
}

void MainComponent::updateFilterMode()
{
	//filter.setMode((int)filterModeSlider.getValue());
	filter.setMode(filterMenu.getSelectedId());
}

void MainComponent::updateA() 
{
	ADSR.updateAttack(attackSlider.getValue());
}
void MainComponent::updateD()
{
	ADSR.updateDecay(decaySlider.getValue());
}
void MainComponent::updateS()
{
	ADSR.updateSustain(sustainSlider.getValue());
}
void MainComponent::updateR()
{
	ADSR.updateRelease(releaseSlider.getValue());
}

void MainComponent::releaseResources()
{
   
}


void MainComponent::paint (Graphics& g)
{
   /*
   Background: 0xff333333
   SliderBackgrond: 0xff555555
   Oscillator: 0xff00805e
   Lfo: 0xff808000
   Filter: 0xff800080
   ADSR: 0xff800000
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
		//frequencySlider.setLookAndFeel();
	int itemMargin = 10;
	Rectangle<int> area = getLocalBounds();
	Rectangle<int> controlArea = area;//.removeFromTop(area.getHeight() / 2);
	Rectangle<int> oscillatorArea = controlArea.removeFromLeft(controlArea.getWidth() / 2);
	Rectangle<int> osc1Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	Rectangle<int> osc2Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	Rectangle<int> lfoArea = oscillatorArea.reduced(itemMargin);
	Rectangle<int> filterArea = (controlArea.removeFromLeft(controlArea.getWidth() / 4)).reduced(itemMargin);
	Rectangle<int> ADSRArea = controlArea.removeFromBottom(controlArea.getHeight()/2).reduced(itemMargin);
	Rectangle<int> plotArea = controlArea.reduced(itemMargin);
	Rectangle<int> plotPlot = plotArea.reduced(itemMargin);
	g.setColour(Colour(0xff444444));
	g.fillRect(osc1Area);
	g.fillRect(osc2Area);
	g.fillRect(lfoArea);
	g.fillRect(filterArea);
	g.fillRect(ADSRArea);
	g.fillRect(plotArea);
	g.setColour(Colour(0xff555555));
	//g.fillRect(plotArea.reduced(itemMargin));
	g.fillRect(plotPlot);
	ADSRStart = plotPlot.getBottomLeft();
	DrawADSR(g);

}

void MainComponent::DrawADSR(Graphics& g) {
	Path path;
	path.startNewSubPath(645,180);

	path.lineTo(750, 100);
	g.setColour(Colour(0xff800000));
	g.fillPath(path);
}
  

void MainComponent::resized()
{
	int itemMargin = 15;

	Rectangle<int> area = getLocalBounds();
	Rectangle<int> controlArea = area;//area.removeFromTop(area.getHeight()/2);
	Rectangle<int> oscillatorArea = controlArea.removeFromLeft(controlArea.getWidth() / 2);
	Rectangle<int> osc1Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	
	
	Rectangle<int> osc2Area = (oscillatorArea.removeFromTop(area.getHeight() / 3)).reduced(itemMargin);
	Rectangle<int> lfoArea = oscillatorArea.reduced(itemMargin);
	Rectangle<int> lfoLabel = lfoArea.removeFromLeft(oscillatorArea.getWidth() / 6);
	Rectangle<int> keyboardArea = area.removeFromBottom(area.getHeight() / 3);
	Rectangle<int> filterArea = (controlArea.removeFromLeft(controlArea.getWidth() / 4)).reduced(itemMargin);
	Rectangle<int> ADSRControlArea = controlArea.removeFromBottom(controlArea.getHeight()/2).reduced(itemMargin); //.removeFromBottom(controlArea.getHeight() / 2);
	
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

	keyboardComponent.setBounds(keyboardArea);

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
}

void MainComponent::initGUI()
{
	
	setWantsKeyboardFocus(true);
	addKeyListener(this);
	grabKeyboardFocus();
	

	//addAndMakeVisible(keyboardComponent);
	//keyboardState.addListener(this);

	addAndMakeVisible(oscillator1_label);
	oscillator1_label.setText("Osc I", NotificationType::dontSendNotification);
	addAndMakeVisible(oscillator2_label);
	oscillator2_label.setText("Osc II", NotificationType::dontSendNotification);
	addAndMakeVisible(lfo_label);
	lfo_label.setText("LFO", NotificationType::dontSendNotification);
	addAndMakeVisible(filter_label);
	filter_label.setText("Filter", NotificationType::dontSendNotification);

	addAndMakeVisible(frequencySlider);
	frequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	frequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	frequencySlider.setRange(-20, 20, 1);
	frequencySlider.setTextValueSuffix(" Hz");
	frequencySlider.addListener(this);
	

	addAndMakeVisible(amplitudeSlider);
	amplitudeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	amplitudeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	amplitudeSlider.setRange(0, 1, 0.01);
	amplitudeSlider.setTextValueSuffix(" dB");
	amplitudeSlider.addListener(this);

	addAndMakeVisible(waveSlider);
	waveSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	waveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	waveSlider.setRange(0, 3, 1);
	waveSlider.addListener(this);

	addAndMakeVisible(frequencySlider2);
	frequencySlider2.setSliderStyle(Slider::SliderStyle::Rotary);
	frequencySlider2.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	frequencySlider2.setRange(-20, 20, 1);
	frequencySlider2.setTextValueSuffix(" Hz");
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
	attackSlider.setRange(0, 1, 0.01);
	attackSlider.addListener(this);

	addAndMakeVisible(decaySlider);
	decaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	decaySlider.setRange(0, 1, 0.01);
	decaySlider.addListener(this);

	addAndMakeVisible(sustainSlider);
	sustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	sustainSlider.setRange(0, 1, 0.01);
	sustainSlider.addListener(this);

	addAndMakeVisible(releaseSlider);
	releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	releaseSlider.setRange(0, 1, 0.01);
	releaseSlider.addListener(this);
}
