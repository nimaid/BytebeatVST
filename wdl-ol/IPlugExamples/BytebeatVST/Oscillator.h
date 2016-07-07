#include <math.h>
#include <stdint.h>
#include <string>

class Oscillator {
private:
	bool isMuted;
	double mFrequency;
	double mCounter;
	double mSampleRate;
	double mCounterIncrement;
	void updateIncrement();
public:
	std::string formula;
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	void resetCounter();
	inline void setMuted(bool muted) { isMuted = muted; }
	double nextSample();
	bool setFormula(std::string goFormula);
	Oscillator() :
		isMuted(true),
		mFrequency(440.0),
		mCounter(0.0),
		mSampleRate(44100.0) {
		updateIncrement();
		//setFormula("t*(42&(t>>11))");
		setFormula("t");
	};
};
