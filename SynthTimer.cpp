#include "Arduino.h"
#include "SynthTimer.h"

SynthTimer::SynthTimer(){}

void SynthTimer::setFrequency(float frequency){
  _dj = int(float(SAMPLE_SIZE)*frequency/BASE_FREQUENCY);
  _dk = int(round(65536.0*fmod(float(SAMPLE_SIZE)*frequency/BASE_FREQUENCY, 1.0)));
}
void SynthTimer::setPitch(float pitch){
  setFrequency(440.0*pow(2.0,pitch/12.0));
}
inline unsigned int SynthTimer::getIndex(){
  return _j;
}
unsigned int SynthTimer::update(){
  _tk = _k + _dk;
  _j += _dj;
  if(_tk<_k) _j++;
  if(_j>=SAMPLE_SIZE) _j-=SAMPLE_SIZE;
  _k = _tk;
  return _j;
}

