#include <math.h>
#include <cstdint>
#include <string>
#include <stack>
#include <deque>
#include "TreeExpressions.h"

class Oscillator {
private:
	bool isMuted;
	double mFrequency;
	double mCounter;
	double mSampleRate;
	double mCounterIncrement;
	void updateIncrement();
	ExpressionTree formulaTree;
public:
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	void resetCounter();
	inline void setMuted(bool muted) { isMuted = muted; }
	double nextSample();
	bool setFormula(std::string formulaIn);
	Oscillator();
};
