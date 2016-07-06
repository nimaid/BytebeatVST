#ifndef __BYTEBEATVST__
#define __BYTEBEATVST__

#include "IPlug_include_in_plug_hdr.h"

class BytebeatVST : public IPlug
{
public:
  BytebeatVST(IPlugInstanceInfo instanceInfo);
  ~BytebeatVST();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double mGain;
};

#endif
