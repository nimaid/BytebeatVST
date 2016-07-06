#include <math.h>

enum OscillatorMode {
	OSCILLATOR_MODE_SINE,
	OSCILLATOR_MODE_SAW,
	OSCILLATOR_MODE_SQUARE,
	OSCILLATOR_MODE_TRIANGLE
};

class Oscillator {
private:
	OscillatorMode mOscillatorMode;
	const double mPI;
	double mFrequency;
	double mPhase;
	double mSampleRate;
	double mPhaseIncrement;
	void updateIncrement();
public:
	void setMode(OscillatorMode mode);
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	void generate(double* buffer, int nFrames);
	Oscillator() :
		mOscillatorMode(OSCILLATOR_MODE_SQUARE),
		mPI(2 * acos(0.0)),
		mFrequency(440.0),
		mPhase(0.0),
		mSampleRate(44100.0) {
		updateIncrement();
	};
};
