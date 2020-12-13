#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioSynthWaveformModulated waveformMod1;   //xy=400.44446563720703,399.99996852874756
AudioSynthWaveformModulated waveformMod2;   //xy=401.00000762939453,433.22221517562866
AudioSynthWaveformSine   sine1;          //xy=401.6666488647461,368.000018119812
AudioEffectEnvelope      envelope1;      //xy=404.3333396911621,464.3333206176758
AudioOutputAnalogStereo  dacs1;          //xy=407.66667556762695,496.55554580688477
AudioConnection          patchCord1(waveformMod1, 0, waveformMod2, 0);
AudioConnection          patchCord2(waveformMod2, envelope1);
AudioConnection          patchCord3(sine1, 0, waveformMod1, 0);
AudioConnection          patchCord4(envelope1, 0, dacs1, 0);
AudioConnection          patchCord5(envelope1, 0, dacs1, 1);
// GUItool: end automatically generated code

Bounce button0 = Bounce(30, 15);

int current_waveform = 0;

extern const int16_t myWaveform[256];  // defined in myWaveform.ino


//GLOBAL VARIABLES || GLOBAL VARIABLES
//GLOBAL VARIABLES || GLOBAL VARIABLES
//GLOBAL VARIABLES || GLOBAL VARIABLES
//GLOBAL VARIABLES || GLOBAL VARIABLES

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

//knob variables
float modulatorLevelKnob = 0;
int modulatorDecayKnob = 0;
int carrierDecayKnob = 0;
int releaseKnob = 0;
int frequencyKnob = 0;

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);

  delay(300);
  Serial.println("Dual Waveform Modulators");

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Comment these out if not using the audio adaptor board.
  //  sgtl5000_1.enable();
  //  sgtl5000_1.volume(0.8); // caution: very loud - use oscilloscope only!

  // Confirgure both to use "myWaveform" for WAVEFORM_ARBITRARY
  waveformMod1.arbitraryWaveform(myWaveform, 172.0);
  waveformMod2.arbitraryWaveform(myWaveform, 344.0);

  // Configure for middle C note without modulation
  waveformMod1.frequency(180);
  waveformMod1.amplitude(1);

  waveformMod2.frequency(90);
  waveformMod2.amplitude(1);

  envelope1.attack(0);
  envelope1.decay(200);
  envelope1.sustain(0.0);

  sine1.frequency(90); // Sine waves are low frequency oscillators (LFO)
  //sine2.frequency(1.2);

  current_waveform = WAVEFORM_SINE;
  waveformMod1.begin(current_waveform);
  waveformMod2.begin(current_waveform);

  // uncomment to try modulating phase instead of frequency
  //waveformMod1.phaseModulation(720.0);
}

void loop() {
  knobHandler();
  buttonHandler();
  cvHandler();

  // Read the buttons and knobs, scale knobs to 0-1.0
  button0.update();
  //  button1.update();
  //  button2.update();
  float knob_A0 = (float)analogRead(A0) / 1023.0;
  float knob_A1 = (float)analogRead(A1) / 1023.0;
  float knob_A2 = map(analogRead(A2), 0, 1023, 1.2, 600);
  float knob_A3 = map(analogRead(A3), 0, 1023, 1.2, 8000);

  // use Knobsto adjust the amount of modulation
  sine1.amplitude(knob_A0);
  //sine2.amplitude(knob_A1);

  sine1.frequency(knob_A2);
  //sine2.frequency(knob_A3);

  if (triggered) {
    envelope1.noteOn();
    envelope1.attack(0);
    envelope1.decay(25);
    envelope1.sustain(0.0);

  }

  // Button 0 or 2 changes the waveform type
  if (button0.fallingEdge()) {
    switch (current_waveform) {
      case WAVEFORM_SINE:
        current_waveform = WAVEFORM_SAWTOOTH;
        Serial.println("Sawtooth");
        break;
      case WAVEFORM_SAWTOOTH:
        current_waveform = WAVEFORM_SAWTOOTH_REVERSE;
        Serial.println("Reverse Sawtooth");
        break;
      case WAVEFORM_SAWTOOTH_REVERSE:
        current_waveform = WAVEFORM_SQUARE;
        Serial.println("Square");
        break;
      case WAVEFORM_SQUARE:
        current_waveform = WAVEFORM_TRIANGLE;
        Serial.println("Triangle");
        break;
      case WAVEFORM_TRIANGLE:
        current_waveform = WAVEFORM_TRIANGLE_VARIABLE;
        Serial.println("Variable Triangle");
        break;
      case WAVEFORM_TRIANGLE_VARIABLE:
        current_waveform = WAVEFORM_ARBITRARY;
        Serial.println("Arbitary Waveform");
        break;
      case WAVEFORM_ARBITRARY:
        current_waveform = WAVEFORM_PULSE;
        Serial.println("Pulse");
        break;
      case WAVEFORM_PULSE:
        current_waveform = WAVEFORM_SAMPLE_HOLD;
        Serial.println("Sample & Hold");
        break;
      case WAVEFORM_SAMPLE_HOLD:
        current_waveform = WAVEFORM_SINE;
        Serial.println("Sine");
        break;
    }
    waveformMod1.begin(current_waveform);
    waveformMod2.begin(current_waveform);
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
