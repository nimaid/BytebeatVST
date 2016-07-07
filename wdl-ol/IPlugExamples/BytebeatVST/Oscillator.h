#include <math.h>
#include <stdint.h>
#include <string>

using namespace std;

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
	const double twoPI;
	bool isMuted;
	double mFrequency;
	double mCounter;
	double mSampleRate;
	double mCounterIncrement;
	void updateIncrement();
	uint8_t evaluateFormula(string evalFormula);
public:
	string formula;
	void setMode(OscillatorMode mode);
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	void resetCounter();
	inline void setMuted(bool muted) { isMuted = muted; }
	double nextSample();
	bool setFormula(string goFormula);
	Oscillator() :
		mOscillatorMode(OSCILLATOR_MODE_SQUARE),
		mPI(2 * acos(0.0)),
		twoPI(2 * mPI), // This line is new
		isMuted(true),  // And this line
		mFrequency(440.0),
		mCounter(0.0),
		mSampleRate(44100.0) {
		updateIncrement();
		setFormula("t*(42&(t>>11))");
	};
};
