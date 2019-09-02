#include <SynthTimer.h>

byte samples[SAMPLE_SIZE];
bool ledpin = false;
unsigned long previousMillis = 0;

int pitch = -36;

SynthTimer synthTimer1;

void setup() {

  //Establish connection with computer
  Serial.begin(9600);
  Serial.println("Initializing");

  //stop interrupts
  cli();

  //set timer2 interrupt at 40kHz
  //Many thanks to Amanda Ghassaei for her Instructibles pages about audio and timer interrupts
  //https://www.instructables.com/id/Arduino-Timer-Interrupts/
  
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; //initialize counter value to 0
  // set compare match register for 40khz increments
  OCR2A = 49; // = (16*10^6) / (40000*8) - 1 (must be <256)
  // turn on CTC (Clear Timer on Compare) mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  //Initialize output pins
  DDRD = B11110000;
  PORTD=0;

  
  Serial.println("Timer initialization complete");

  
  initializeSample();
  synthTimer1.setFrequency(44.0);

  pinMode(13,OUTPUT);

  sei();

  
  Serial.println("Ready");
}


void initializeSample(){
  for(int i = 0; i < SAMPLE_SIZE; i++){

    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis-previousMillis>100){
    if(ledpin){
      digitalWrite(13, HIGH);
    }
    else{
      digitalWrite(13, LOW);
      Serial.println(pitch);
      synthTimer1.setPitch(float(pitch++));
      if(pitch>30) pitch = -30;
      
    }
    ledpin = !ledpin;
    previousMillis=currentMillis;
  }
}

ISR(TIMER2_COMPA_vect){//timer2 interrupt 40kHz
  //This code runs 40,000 times per second
  
  PORTD = samples[synthTimer1.update()];
}
