/* This is a quad gate generator that uses swing and clock rotation
    to generate
*/

//photocell pin is a
int photocellPin = A2;
int cvReading;

int pot1Reading;
int pot2Reading;

int ledPin0 = 2;
int ledPin1 = 3;
int ledPin2 = 4;
int ledPin3 = 5;

int delayIntervals[] = {8000, 4000, 2000, 500};

int swingAmount = 0;

int ledState0 = LOW;
int ledState1 = LOW;
int ledState2 = LOW;
int ledState3 = LOW;
unsigned long previousMillis0 = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long interval0 = 2000;
unsigned long interval1 = 1500;
unsigned long interval2 = 1250;
unsigned long interval3 = 1125;

void setup() {
  Serial.begin(9600);
  // Gate/LED pins
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);


}
void loop() {
  unsigned long currentMillis = millis();
  //Serial.println(currentMillis);

  pot1Reading = map(analogRead(A0), 1023, 0, 0, 500);
  pot2Reading = map(analogRead(A1), 1023, 0, 1, 15);

  cvReading = map(analogRead(A2), 0, 1023, 0, 1000);
  //Serial.println(cvReading);
  int randomNumber = random(100);
  Serial.println(randomNumber);
  if (cvReading <= 250) {

    if (randomNumber > 3) {
      interval0 = 6000 / pot2Reading;
      interval1 = 12000 / pot2Reading;
      interval2 = 3000 / pot2Reading;
      interval3 = 1000 / pot2Reading;
    } else {
      interval0 = 6000 / pot2Reading * 2;
      interval1 = 12000 / pot2Reading * 2;
      interval2 = 3000 / pot2Reading * 2;
      interval3 = 1000 / pot2Reading * 2;
    }

  } else if (cvReading >= 251 && cvReading <= 750) {
    if (randomNumber > 3) {
      interval0 = 1000 / pot2Reading;
      interval1 = 6000 / pot2Reading;
      interval2 = 12000 / pot2Reading;
      interval3 = 3000 / pot2Reading;
    } else {
      interval0 = 1000 / pot2Reading * 2;
      interval1 = 6000 / pot2Reading * 2;
      interval2 = 12000 / pot2Reading * 2;
      interval3 = 3000 / pot2Reading * 2;
    }

  } else if (cvReading >= 751 && cvReading <= 1000) {
    if (randomNumber > 3) {
      interval0 = 3000 / pot2Reading;
      interval1 = 1000 / pot2Reading;
      interval2 = 6000 / pot2Reading;
      interval3 = 12000 / pot2Reading;
    } else {
      interval0 = 3000 / pot2Reading * 2;
      interval1 = 1000 / pot2Reading * 2;
      interval2 = 6000 / pot2Reading * 2;
      interval3 = 12000 / pot2Reading * 2;
    }

  } else {
    if (randomNumber > 3) {
      interval0 = 12000 / pot2Reading * 2;
      interval1 = 3000 / pot2Reading * 2;
      interval2 = 1000 / pot2Reading * 2;
      interval3 = 6000 / pot2Reading * 2;
    } else {
      interval0 = 12000 / pot2Reading * 2;
      interval1 = 3000 / pot2Reading * 2;
      interval2 = 1000 / pot2Reading * 2;
      interval3 = 6000 / pot2Reading * 2;
    }

  }

  // Serial.println(pot2Reading);

  if (currentMillis - previousMillis0 + swingAmount > interval0) {
    previousMillis0 = currentMillis;
    if (ledState0 == LOW)
      ledState0 = HIGH;
    else
      ledState0 = LOW;
    digitalWrite(ledPin0, ledState0);
  }
  if (currentMillis - previousMillis1 + swingAmount > interval1) {
    previousMillis1 = currentMillis;
    if (ledState1 == LOW)
      ledState1 = HIGH;
    else
      ledState1 = LOW;
    digitalWrite(ledPin1, ledState1);
  }
  if (currentMillis - previousMillis2 + swingAmount > interval2) {
    previousMillis2 = currentMillis;
    if (ledState2 == LOW)
      ledState2 = HIGH;
    else
      ledState2 = LOW;
    digitalWrite(ledPin2, ledState2);
  }
  if (currentMillis - previousMillis3 + swingAmount > interval3) {
    previousMillis3 = currentMillis;
    if (ledState3 == LOW)
      ledState3 = HIGH;
    else
      ledState3 = LOW;
    digitalWrite(ledPin3, ledState3);
  }

}
