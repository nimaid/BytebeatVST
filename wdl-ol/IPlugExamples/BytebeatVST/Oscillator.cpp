#include "Oscillator.h"

//Bytebeats are 0-255, or 8 bit
#define FORMULA_MODULO (double)(1 << 8)

void Oscillator::setMode(OscillatorMode mode) {
	mOscillatorMode = mode;
}

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

uint8_t Oscillator::evaluateFormula(string evalFormula)
{

}

bool Oscillator::setFormula(string goFormula)
{
	
	string allowedSubstrings[] = {"(", ")", "t", "<<", ">>", "&", "|", "^", "*", "+", "-", "/", "%"};

	for(int i = 0; i < goFormula.length(); i++)
	{
		for(int j = 0; j < allowedSubstrings)
	}

	formula == goFormula;
}

double Oscillator::nextSample() {
	double value = 0.0;
	uint32_t t;
	uint8_t result;

	if (isMuted) return value;
	
	// Convert running float to a 32 bit uint
	t = (uint32_t)mCounter;

	// Main formula
	result =(uint8_t)(t*(42&(t>>11)));

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