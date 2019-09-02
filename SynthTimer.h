#ifndef SynthTimer_h
#define SynthTimer_h

#include "Arduino.h"
#include "SynthConstants.h"


class SynthTimer{
  public:
    SynthTimer();
    void setFrequency(float frequency);
    void setPitch(float pitch);
    unsigned int getIndex();
    unsigned int update();
   private:
    unsigned int _j = 0; //Index of the sample being played
    unsigned int _k = 0; //Sub-index of the sample being played
    unsigned int _dj = 0; //# to add to j each 1/40000th of a second
    unsigned int _dk = 0; //# to add to k each 1/40000th of a second
    unsigned int _tj = 0; //# Temporary variable for j
    unsigned int _tk = 0; //# Temporary variable for k
};

#endif
