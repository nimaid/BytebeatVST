#ifndef __BYTEBEATVST__
#define __BYTEBEATVST__

#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"

class BytebeatVST : public IPlug
{
public:
  BytebeatVST(IPlugInstanceInfo instanceInfo);
  ~BytebeatVST();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double mFrequency;
  void CreatePresets();
  Oscillator mOscillator;
};

#endif
