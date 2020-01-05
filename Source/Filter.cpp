

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

Filter::Filter()
{
	cutoff = 0.99;
	resonance = 0;
	mode = 1;
	buf0 = 0;
	buf1 = 0;
	buf2 = 0;
	buf3 = 0;
	calculateFeedback();
}

Filter::~Filter()
{
}

double Filter::process(double inputValue)
{
	/* two first order filters in series, using commented variables adds two more filters in series*/

	buf0 += cutoff * (inputValue - buf0 + feedback * (buf0 - buf1));
	buf1 += cutoff * (buf0 - buf1);
	//buf2 += cutoff * (buf1 - buf2);
	//buf3 += cutoff * (buf2 - buf3);
	switch (mode) {
		case 1:	//Lowpass
			return buf1;
		case 2:	//Highpass
			return inputValue - buf0;
		case 3:	//Bandpass
			return buf0 - buf1;

	}
}

void Filter::setCutoff(double newCut)
{
	if (newCut < 1)
	{
		cutoff = newCut;
		calculateFeedback();
	}
}

void Filter::setResonance(double newRes)
{
	resonance = newRes;
	calculateFeedback();
}

void Filter::setMode(int newMode)
{
	mode = newMode;
}

void Filter::calculateFeedback()
{
	feedback = resonance + resonance / (1 - cutoff);
}

double Filter::getCutoff()
{
	return cutoff;
}
