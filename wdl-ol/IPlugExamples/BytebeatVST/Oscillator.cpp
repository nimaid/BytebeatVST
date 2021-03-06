#include "Oscillator.h"

//Bytebeats are 0-255, or 8 bit
#define FORMULA_MODULO (double)(1 << 8)

Oscillator::Oscillator()
{
	isMuted = true;
	mFrequency = 440.0;
	mCounter = 0.0;
	mSampleRate = 44100.0;
	updateIncrement();
	//setFormula("t");
	setFormula("(t >> 8 & t) * t");
	//setFormula("(t>>8&t)*t");
	//setFormula("t*(42&(t>>11))");
	//setFormula("t*((42&(t>>11))%24)");
	//setFormula("(t)*5&((t)>>7)|(t*3)&(t*4>>10)");
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

bool Oscillator::setFormula(std::string formulaIn) {
	formulaTree.build(formulaIn);
	return(true);
}

double Oscillator::nextSample() {
	double value = 0.0;
	double result;

	if (isMuted) return value;

	uint32_t t = (uint32_t)mCounter;

	// Main computation
	result = (double)formulaTree.evaluate(t);

	//Formula conditioning/normalization
	while (result >= (FORMULA_MODULO / 2.))
	{
		result -= FORMULA_MODULO;
	}

	value = (2. * (double)result) / FORMULA_MODULO;


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