#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

//buttonHandler variables
int buttonPin = 30;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
bool buttonPushed = false;

//cvHandler variables
bool previousCvState = false;
bool cvState = false;
int cvReading = 0;
bool triggered = false;

int bitCrushBits = 0;
int bitCrushSampleRate = 0;

float modulatorLevelKnob = 0;
int modulatorDecayKnob = 0;
int carrierDecayKnob = 0;
int releaseKnob = 0;

int frequencyKnob = 0;

// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc1;            //xy=205.55554962158203,389.99999141693115
AudioSynthWaveformSineModulated sinFM;          //xy=330.55554962158203,390.0000123977661
AudioSynthWaveformSineModulated sinFM2;       //xy=465.22222900390625,389
AudioEffectEnvelope      out;            //xy=612.1110610961914,391.1111259460449
AudioOutputAnalogStereo  dacs2;          //xy=784.666633605957,388.88888025283813
AudioConnection          patchCord1(dc1, sinFM);
AudioConnection          patchCord2(sinFM, sinFM2);
AudioConnection          patchCord3(sinFM2, out);
AudioConnection          patchCord4(out, 0, dacs2, 0);
AudioConnection          patchCord5(out, 0, dacs2, 1);
// GUItool: end automatically generated code

void setup() {
  pinMode(buttonPin, INPUT);
  AudioMemory(40);
  dc1.amplitude(1);
  //env.attack(0);
  //env.decay(80);
  //env.sustain(0.0);
  out.attack(0);
  out.decay(180);
  out.sustain(0.0);
  out.release(1);
  //amp1.gain(1);
  sinFM.frequency(90);
  sinFM.amplitude(1);

  sinFM2.frequency(90);
  sinFM2.amplitude(1);

  //bitcrusher1.bits(16);
  //bitcrusher1.sampleRate(44100);
}

void loop() {
  knobHandler();
  buttonHandler();
  cvHandler();

  //bitcrusher1.bits(bitCrushBits);
  // bitCrushSampleRate = map(analogRead(A1), 0, 1023, 1000, 44100);
  //bitcrusher1.sampleRate(bitCrushSampleRate);
  sinFM.frequency(frequencyKnob);
  sinFM2.frequency(frequencyKnob);

  if (triggered) {
    //Serial.println("The button was pressed");
    // env.noteOn();
    out.noteOn();
    //env.attack(0);
    out.attack(0);
    // env.decay(modulatorDecayKnob);
    out.decay(carrierDecayKnob);
    // env.sustain(0);
    out.sustain(0);
    //amp1.gain(modulatorLevelKnob);
    // env.release(0);
    // out.release(0);
    //env.noteOff();
    out.noteOff();
    Serial.print("Modulator Level Val: ");
    Serial.println(modulatorLevelKnob);
    Serial.print("Modulator Decay Val: ");
    Serial.println(modulatorDecayKnob);
    Serial.print("Frequency Val: ");
    Serial.println(frequencyKnob);
    Serial.print("Carrier Decay Val: ");
    Serial.println(carrierDecayKnob);

  }
}

void buttonHandler() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
      buttonPushed = true;
      //  Serial.print("The last button state is ");
      //  Serial.println(lastButtonState);
      //  Serial.print("The button state is ");
      //  Serial.println(buttonState);
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

void cvHandler() {
  cvReading = analogRead(A14);
  if (cvReading > 350) {
    cvState = true;
  } else {
    cvState = false;
  }
  if (cvState != previousCvState) {
    if (cvState == true) {
      triggered = true;
    } else {
      //buttonPushed = false;
      triggered = false;
    }
    delay (10);
  }
  previousCvState = cvState;
}

void knobHandler() {
  modulatorLevelKnob = map(analogRead(A0), 0, 1023, 0.0, 32767.0);
  modulatorDecayKnob = map(analogRead(A1), 0, 1023, 0, 10000);
  frequencyKnob = map(analogRead(A2), 0, 1023, 40, 120);
  carrierDecayKnob = map(analogRead(A3), 0, 1023, 0, 10000);

}
