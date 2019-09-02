#include "Arduino.h"
#include "Instrument.h"


Instrument::Instrument(){}
void Instrument::setTimber(float sine, float triangle, float sawtooth, float square, float noise){
	for(int i = 0; i < SAMPLE_SIZE; i++){



		float fi = float(i)/float(SAMPLE_SIZE);

		float sample = 0.0; //-1.0 -> 0, 1.0 -> 256
		//Add sin function
		sample += sine * sin(fi*2.0*PI);

		//Add triangle function
		if(sample<SAMPLE_SIZE/2){
			sample += triangle*(fi*4.0-1.0);
		}else{
			sample += triangle*(float(SAMPLE_SIZE-i)*4.0/SAMPLE_SIZE-1.0);
		}

		//add sawtooth function
		sample += sawtooth * (fi * 2.0 - 1.0);

		//add square function
		if(i<SAMPLE_SIZE/2){
			sample += -square;
		}else{
			sample += square;
		}

		//add random funtion
		sample += noise * (float) random() / 2147483647.0;

		//Compress the sample into a byte
		_waveform[i] = byte(127.0*sample+127.0);


		//_waveform[i] = sin(fi*2.0*PI)*127.0+127.0;



		//Generate samples of a sawtooth wave
		//_waveform[i] = i;
	}
}
void Instrument::setFrequency(int index, float frequency){
	_timers[index].setFrequency(frequency);
}
void Instrument::setPitch(int index, float pitch){
	_timers[index].setPitch(pitch);
}
void Instrument::enableNote(byte index){
	if(~_enabled & 1<<index) _count++;
	_enabled |= 1<<index;
}
void Instrument::disableNote (byte index){
	if(_enabled & 1<<index) _count--;
	_enabled ^= 1<<index;
}
void Instrument::setVolume(byte volume){
	_volume = volume;
}
uint8_t Instrument::getSample(){
	return (uint8_t) _sample;
}
uint8_t Instrument::update(){

	if(_enabled & 1) _sample = _waveform[_timers[0].update()];
	else _sample = 0;
	if(_enabled & 2) _sample += _waveform[_timers[1].update()];
	if(_enabled & 4) _sample += _waveform[_timers[2].update()];
	if(_enabled & 8) _sample += _waveform[_timers[3].update()];


	 /*//Too slow
	_sample = 0;
	for(_i = 0; _i < NUM_TONES; _i++){
		if(_enabled & 1<<_i){
			_sample += _timers[_i].update();
		}
	}
	*/


	_sample = ((_sample-(128*_count)) >> _volume) + 127;
	return (uint8_t) _sample;
}
