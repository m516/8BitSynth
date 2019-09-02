#define SAMPLE_SIZE 256
#define BASE_FREQUENCY 40000

unsigned int j = 0; //Index of the sample being played
unsigned int k = 0; //Sub-index of the sample being played
unsigned int dj = 0; //# to add to j each 1/40000th of a second
unsigned int dk = 0; //# to add to k each 1/40000th of a second

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setFrequency(440.0);
  Serial.print("dj: ");
  Serial.println(dj);
  Serial.print("dk: ");
  Serial.println(dk);
}

void setFrequency(float frequency){
  dj = int(float(SAMPLE_SIZE)*frequency/BASE_FREQUENCY);
  dk = int(round(65536.0*fmod(float(SAMPLE_SIZE)*frequency/BASE_FREQUENCY, 1.0)));
}

void loop() {
  // put your main code here, to run repeatedly:

}
