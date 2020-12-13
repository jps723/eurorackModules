#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//buttonHandler variables
int buttonPin = 30;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
bool buttonPushed = false;

// GUItool: begin automatically generated code
AudioEffectEnvelope      envelope2;      //xy=116,225
AudioSynthWaveformSineModulated sine_fm1;       //xy=267,225
AudioEffectEnvelope      envelope1;      //xy=418,225
AudioEffectBitcrusher    bitcrusher1;    //xy=571,225
AudioOutputAnalogStereo  dacs1;          //xy=711,224
AudioConnection          patchCord1(envelope2, sine_fm1);
AudioConnection          patchCord2(sine_fm1, envelope1);
AudioConnection          patchCord3(envelope1, bitcrusher1);
AudioConnection          patchCord4(bitcrusher1, 0, dacs1, 0);
AudioConnection          patchCord5(bitcrusher1, 0, dacs1, 1);
// GUItool: end automatically generated code

void setup() {
  Serial.begin(115200);
  // audio library init
  AudioMemory(15);
  //next = millis() + 250;
  pinMode(buttonPin, INPUT);
  AudioNoInterrupts();

  envelope1.attack(30);
  envelope1.decay(1500);
  envelope1.sustain(0.8);
  envelope1.release(1400);

  envelope2.attack(30);
  envelope2.decay(150);
  envelope2.sustain(0.8);
  envelope2.release(1400);

  sine_fm1.amplitude(.85);
  sine_fm1.frequency(440);

  bitcrusher1.bits(3);
  bitcrusher1.sampleRate(44100);
  delay(500);
}

void loop() {
  buttonHandler();
  if (buttonPushed) {
    //playKick();
        sine_fm1.amplitude(.85);
    sine_fm1.frequency(440);
    envelope2.noteOn();
    //envelope2.delay(50);
    delay(250);
    envelope2.attack(30);
    envelope2.decay(150);
    envelope2.sustain(0.8);
    envelope2.release(1400);
    envelope2.noteOff();
    delay(250);



    envelope1.noteOn();
    delay(250);
    envelope2.delay(50);
    envelope1.attack(30);
    envelope1.decay(150);
    envelope1.sustain(0.8);
    envelope1.release(1400);
    envelope1.noteOff();
    delay(250);
  }



}

void buttonHandler() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      //playKick();
      Serial.println("the button was pressed");

      buttonPushCounter++;
      buttonPushed = true;
    } else {
      buttonPushed = false;
      //if the button isn't pressed, don't do anything
    }
    delay(10);

  }
  lastButtonState = buttonState;


  //  switch (buttonPushCounter) {
  //    case 0:
  //      drumType = "HAT.WAV";
  //      break;
  //    case 1:
  //      drumType = "KICK.WAV";
  //      break;
  //    case 2:
  //      drumType = "SNAP.WAV";
  //      break;
  //    case 3:
  //      drumType = "SNARE.WAV";
  //      break;
  //  }
  //  if (buttonPushCounter >= 4) {
  //    buttonPushCounter = 0;
  //  }
}

void playKick() {
  envelope2.noteOn();
  //envelope2.delay(50);
  delay(250);
  //  envelope2.attack(30);
  //  envelope2.decay(150);
  //  envelope2.sustain(0.8);
  //  envelope2.release(1400);
  envelope2.noteOff();
  delay(250);

  //  sine_fm1.amplitude(.85);
  //  sine_fm1.frequency(440);

  envelope1.noteOn();
  delay(250);
  //envelope2.delay(50);
  //  envelope1.attack(30);
  //  envelope1.decay(150);
  //  envelope1.sustain(0.8);
  //  envelope1.release(1400);
  envelope1.noteOff();
  delay(250);


}
