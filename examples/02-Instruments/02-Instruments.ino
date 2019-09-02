#include <SynthTimer.h>
#include <Instrument.h>
#include <SynthConstants.h>

byte samples[SAMPLE_SIZE];
bool ledpin = false;
unsigned long previousMillis = 0;


//int pitch = 0;

Instrument instrument;
SynthTimer synthTimer1;

//Pitch of each note played by the instrument
int pitches[]   = { -1, -1, -1, -1};
byte channels[] = {0, 1, 2, 3}; //there are 16 channels (instruments): 0-15
byte indexOfChannelChange = 0;

void setup() {

  //Establish connection with computer
  Serial.begin(9600);
  Serial.println("Initializing");

  //Establish connection with MIDI device
  Serial1.begin(31250);

  //stop interrupts
  cli();

  //set timer2 interrupt at 16kHz
  //Many thanks to Amanda Ghassaei for her Instructibles pages about audio and timer interrupts
  //https://www.instructables.com/id/Arduino-Timer-Interrupts/

  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; //initialize counter value to 0
  // set compare match register for 40khz increments
  OCR2A = 79; // = (16*10^6) / (40000*8) - 1 (must be <256)
  // turn on CTC (Clear Timer on Compare) mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  //Initialize output pins
  DDRA = B11111111;
  PORTA = 0;


  Serial.println("Timer initialization complete");

  //Set what the the instrument sounds like
  //                   sine  triangle sawtooth square noise
  instrument.setTimber(0.05, 0.05,    0.6,     0.05,  0.05);
  /*
    instrument.setPitch(0,0.0);
    instrument.setPitch(1,4.0);
    instrument.setPitch(2,7.0);
    instrument.setPitch(3,12.0);
    for(int i = 0; i < NUM_TONES; i++){
    instrument.enableNote(i);
    }
  */
  instrument.setVolume(2);

  pinMode(13, OUTPUT);

  sei();


  Serial.println("Ready");
}


void loop() {
  //Read serial
  while (Serial1.available() > 2) {
    byte command = Serial1.read(); //read first byte
    byte note = Serial1.read();    //read next byte
    byte velocity = Serial1.read();//read final byte

    //First check if this channel is occupied
    bool hasChannel = false;
    byte currentChannel = command & B00001111;
    for (int i = 0; i < 4; i ++) {
      if(channels[i]==currentChannel){
        hasChannel = true;
        break;
      }
    }
    if(!hasChannel){
      Serial.print("Changing channel with index ");
      Serial.print(indexOfChannelChange);
      Serial.print(" from ");
      Serial.print(channels[indexOfChannelChange]);
      Serial.print(" to ");
      Serial.println(currentChannel);
      channels[indexOfChannelChange++]=currentChannel;
      if(indexOfChannelChange>=4) indexOfChannelChange=0;
    }



    for (int i = 0; i < 4; i ++) {
      if (command == (B10010000 + channels[i])) { //note on
        if (velocity == 0) { //actually disabling note
          instrument.disableNote(i);
          break;
        }
        else {
          instrument.enableNote(i);
          instrument.setPitch(i, float(note) - 69.0);
          break;
        }
      }
      else if (command == (B10000000 + channels[i])) { //note off
        instrument.disableNote(i);
        break;
      }
      else if (command == (B10110000 + channels[i])) { //Control, usually means turn off all sounds
        //instrument.disableNote(i);
      }
    }
  }

  /*
    //Try polyphony
    unsigned long currentMillis = millis();
    if(currentMillis-previousMillis>1000){
    if(ledpin){
      digitalWrite(13, HIGH);
    }
    else{
      digitalWrite(13, LOW);
      Serial.println(pitch);
      instrument.setPitch(0, float(pitch));
      instrument.setPitch(1, float(pitch+4));
      instrument.setPitch(2, float(pitch+7));
      instrument.setPitch(3, float(pitch-24));
      pitch++;
      if(pitch>6) pitch = -6;

    }
    ledpin = !ledpin;
    previousMillis=currentMillis;
    }
  */
}

ISR(TIMER2_COMPA_vect) { //timer2 interrupt 40kHz
  //This code runs 40,000 times per second

  PORTA = instrument.update();
}
