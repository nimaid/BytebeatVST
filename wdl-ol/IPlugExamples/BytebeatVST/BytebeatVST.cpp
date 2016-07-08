#include "BytebeatVST.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 5;

enum EParams
{
  //kFrequency = 0,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,
  /*
  kKnobX = 48,
  kKnobY = 48,

  kFrequencyX = (kWidth - kKnobX) / 2,
  kFrequencyY = (kHeight - kKnobY) / 2,
  kKnobFrames = 60
  */
};

BytebeatVST::BytebeatVST(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mFrequency(1.)
{
  TRACE;

  /*
  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kFrequency)->InitDouble("Frequency", 440.0, 50.0, 20000.0, 0.01, "Hz");
  GetParam(kFrequency)->SetShape(2.);
  */

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  //pGraphics->AttachPanelBackground(&COLOR_RED);
  pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);
  /*
  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kFrequencyX, kFrequencyY, kFrequency, &knob));
  */
  AttachGraphics(pGraphics);

  CreatePresets();
}

BytebeatVST::~BytebeatVST() {}

void BytebeatVST::CreatePresets()
{
	//MakePreset("Clean", 440.);
	MakeDefaultPreset((char *) "-", kNumPrograms);
}

void BytebeatVST::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.
	
	double *leftOutput = outputs[0];
	double *rightOutput = outputs[1];

	for (int i = 0; i < nFrames; ++i) {
		mMIDIReceiver.advance();
		int velocity = mMIDIReceiver.getLastVelocity();
		if (velocity > 0) {
			mOscillator.setFrequency(mMIDIReceiver.getLastFrequency());
			mOscillator.setMuted(false);
		}
		else {
			mOscillator.setMuted(true);
		}
		leftOutput[i] = rightOutput[i] = mOscillator.nextSample() * velocity / 127.0;
	}

	mMIDIReceiver.Flush(nFrames);
}

void BytebeatVST::Reset()
{
  TRACE;
  IMutexLock lock(this);
  mOscillator.setSampleRate(GetSampleRate());
}

void BytebeatVST::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  /*
  switch (paramIdx)
  {
    case kFrequency:
		mOscillator.setFrequency(GetParam(kFrequency)->Value());
      break;

    default:
      break;
  }
  */
}

void BytebeatVST::ProcessMidiMsg(IMidiMsg* pMsg) {
	if ((pMsg->StatusMsg() == IMidiMsg::kNoteOff) && (pMsg->NoteNumber() == mMIDIReceiver.mLastNoteNumber))
	{
		// Reset the oscillator counter if note off and there isn't another note being played
		mOscillator.resetCounter();
	}

	mMIDIReceiver.onMessageReceived(pMsg);
}