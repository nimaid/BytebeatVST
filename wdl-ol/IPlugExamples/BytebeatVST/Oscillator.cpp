#include "Oscillator.h"

//Bytebeats are 0-255, or 8 bit
#define FORMULA_MODULO (double)(1 << 8)

void Oscillator::setFrequency(double frequency) {
	mFrequency = frequency;
	updateIncrement();
}

void Oscillator::setSampleRate(double sampleRate) {
	mSampleRate = sampleRate;
	updateIncrement();
}

void Oscillator::updateIncrement() {
	// This will be used to make mCounter go linearly in range [0, 256) at mFrequency
	mCounterIncrement = (mFrequency * FORMULA_MODULO) / mSampleRate;
}

void Oscillator::resetCounter() {
	mCounter = 0.;
}

bool Oscillator::setFormula(std::string goFormula) {
	formula == goFormula;
	return(true);
}

double Oscillator::nextSample() {
	double value = 0.0;
	uint32_t t;
	uint8_t result;

	if (isMuted) return value;

	t = (uint32_t)mCounter;

	// Main computation
	result = (uint8_t)(t*(42&(t>>11)));

	/*HERE IS WHERE I NEED TO GET THE result VAR FROM PYTHON*/

	//Formula conditioning/normalization
	value = ((2. * (double)result) / FORMULA_MODULO) - 1.;

	// Increment and float modulo
	mCounter += mCounterIncrement;
	
	/*
	while (mCounter >= (double)(1 << 32))
	{
		mCounter -= (double)(1 << 32);
	}
	*/
	
	return value;
}