/* A eurorack sampler for teensy 3.6.

*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "samples.h"

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=106,362
AudioPlaySdWav           playSdWav2;     //xy=106,462
AudioMixer4              mixer1;         //xy=275,394
AudioEffectFreeverbStereo freeverbs1;     //xy=433,319
AudioMixer4              mixer3;         //xy=539,546
AudioMixer4              mixer2;         //xy=614,361
AudioOutputAnalogStereo  dacs2;          //xy=703,456
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer1, 1);
AudioConnection          patchCord3(playSdWav2, 0, mixer1, 2);
AudioConnection          patchCord4(playSdWav2, 1, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 1);
AudioConnection          patchCord6(mixer1, freeverbs1);
AudioConnection          patchCord7(mixer1, 0, mixer3, 1);
AudioConnection          patchCord8(freeverbs1, 0, mixer2, 0);
AudioConnection          patchCord9(freeverbs1, 1, mixer3, 0);
AudioConnection          patchCord10(mixer3, 0, dacs2, 1);
AudioConnection          patchCord11(mixer2, 0, dacs2, 0);
// GUItool: end automatically generated code


// Use these with the Teensy Audio Shield
//#define SDCARD_CS_PIN    10
//#define SDCARD_MOSI_PIN  7
//#define SDCARD_SCK_PIN   14

// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

//buttonHandler variables
int buttonPin = 30;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

//Sample Selection Variable
char *drumType = "blah";
bool buttonPushed = false;

//cvHandler variables
bool previousCvState = false;
bool cvState = false;
int cvReading = 0;
bool triggered = false;

//reverb variables
float dryWet = 0.5;
float roomSize = 0.9;
float damp = 0.3;



void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  //  sgtl5000_1.enable();
  //  sgtl5000_1.volume(0.5);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer2.gain(0, 0.9); // hear 90% "wet"
  mixer2.gain(1, 0.1); // and  10% "dry"
  mixer3.gain(0, 0.9);
  mixer3.gain(1, 0.1);
}

void loop() {
  buttonHandler();
  cvHandler();

  if (triggered) {
    if (playSdWav1.isPlaying() == false) {
      playFile(samples[random(60)+1]);
      delay(10);
    } else {
      //playSdWav1.stop();
      delay(10);
      playFile2(samples[random(60)]+1);
      delay(10);
    }
  }

  mixer2.gain(0, dryWet);
  mixer2.gain(1, 1.0 - dryWet);
  mixer3.gain(0, dryWet);
  mixer3.gain(1, 1.0 - dryWet);
  freeverbs1.roomsize(roomSize);
  freeverbs1.damping(damp);

  dryWet = (float)analogRead(A0) / 1023.0;
  roomSize = (float)analogRead(A1) / 1023.0;
  damp = (float)analogRead(A2) / 1023.0;
  //  Serial.print("Reverb: mix=");
  //  Serial.print(dryWet * 100.0);
  //  Serial.print("%, roomsize=");
  //  Serial.print(roomSize * 100.0);
  //  Serial.print("%, damping=");
  //  Serial.print(damp * 100.0);
  //  Serial.print("%, CPU Usage=");
  //  Serial.print(freeverbs1.processorUsage());
  //  Serial.println("%");

  //Serial.println(buttonPushCounter);
}

void playFile(char *filename) {
  playSdWav1.play(filename);
  while (playSdWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  }
}

void playFile2(char *filename) {
  playSdWav2.play(filename);
  while (playSdWav2.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  }
}

void buttonHandler() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      buttonPushed = true;
    } else {
      buttonPushed = false;
      //if the button isn't pressed, don't do anything
    }
    delay(10);
  }
  lastButtonState = buttonState;

  switch (buttonPushCounter) {
    case 0:
      drumType = "HAT.WAV";
      break;
    case 1:
      drumType = "KICK.WAV";
      break;
    case 2:
      drumType = "SNAP.WAV";
      break;
    case 3:
      drumType = "SNARE.WAV";
      break;
  }
  if (buttonPushCounter >= 4) {
    buttonPushCounter = 0;
  }
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
