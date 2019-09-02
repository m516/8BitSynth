#ifndef Instrument_h
#define Instrument_h

#include "Arduino.h"
#include "SynthTimer.h"
#include "SynthConstants.h"



class Instrument{
  public:
    Instrument();
	void setTimber(float sine, float triangle, float sawtooth, float square, float noise);
    void setFrequency(int index, float frequency);
    void setPitch(int index, float pitch);
	void enableNote(byte index);
	void disableNote (byte index);
	void setVolume(byte volume);
    byte getSample();
    byte update();
   private:
    SynthTimer _timers[NUM_TONES];
	byte _enabled = 0;
	byte _waveform[SAMPLE_SIZE];
	byte _volume = 0;
	int _sample;
	byte _i = 0;
	byte _count = 0;
};

#endif