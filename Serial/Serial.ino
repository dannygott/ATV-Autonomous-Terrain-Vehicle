#include <Servo.h>

// Servo objects
Servo throttleServo;
Servo turnSpeedController;
// Indicator Pins
// TODO: Change values to actual pins on the board
int lightSolidPin = 0;
int lightBlinkPin = 0;
int honkPin = 0;
// Encoder Pins
int encoder0PinA = 32;
int encoder0PinB = 33;
int encoder0Pos = 0;
// Shifting Pins
int shiftupPin = 24;
int shiftupStop = 25;
int shiftdownPin = 23;
int shiftdownStop = 22;
// Potentiometer pin for Turning
int turnPotPin = 0;
// Various value storage
int encoder0PinALast = LOW;
int servoPoz = 0;
int n = LOW;
int shiftVal = 0;
int prevShiftVal = 0;
int shiftPoz = 1;
int previousMillis = -1;
int tBetweenPulse = 0;
int previousEncoder0Pos = 0;
int turnValue = 180;
// Indicates status of entire system. 0 = Stopped, 1 = Running, 2 = Error
int systemStatus = 0;

void setup() {
  pinMode(encoder0PinA, INPUT_PULLUP); // Dont forget to change
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(shiftupPin, OUTPUT);
  pinMode(shiftdownPin, OUTPUT);
  Serial.begin(2000000);
  throttleServo.attach(40);
  turnSpeedController.attach(41);
  Serial.setTimeout(10);
}

void loop() {
  // If anything comes in Serial (USB)
  if (Serial.available()) {

    servoPoz = Serial.readStringUntil(',').toInt();
    shiftVal = Serial.readStringUntil(',').toInt();
    turnValue = Serial.readStringUntil(',').toInt();

    Serial.println(encoder0Pos);
    Serial.println(shiftPoz);
    Serial.println(handleEncoderStop(encoder0Pos));
    // Serial.println(systemStatus); //TODO: Implement Later
  }
  handleTurn(turnValue);
  handleShift(shiftVal);
  handleEncoder(digitalRead(encoder0PinA));
  throttleServo.write(servoPoz);
}

int handleEncoderStop(int encoderVal) {

  if (previousEncoder0Pos == encoderVal) {
    tBetweenPulse = 0;
  }
  previousEncoder0Pos = encoder0Pos;
  return tBetweenPulse;
}

void handleShift(int shiftValue) {
  switch (shiftValue) {
  case 0:
    if (prevShiftVal == 1) {
      shiftPoz--;
      prevShiftVal = 0;
    } else if (prevShiftVal == 2) {
      shiftPoz++;
      prevShiftVal = 0;
    }
    digitalWrite(shiftupPin, LOW);
    digitalWrite(shiftdownPin, LOW);
    digitalWrite(shiftdownStop, HIGH);
    digitalWrite(shiftupStop, HIGH);
    break;
  case 1:
    if (shiftPoz > 0) {
      digitalWrite(shiftupStop, LOW);
      digitalWrite(shiftdownPin, HIGH);
      prevShiftVal = shiftValue;
    }
    break;

  case 2:
    if (shiftPoz < 3) {
      digitalWrite(shiftupStop, LOW);
      digitalWrite(shiftupPin, HIGH);
      prevShiftVal = shiftValue;
    }
    break;
  default:
    break;
  }
}

void handleEncoder(int encoderPinVal) {
  if ((encoder0PinALast == LOW) && (encoderPinVal == HIGH)) {

    if (digitalRead(encoder0PinB) == LOW) {
      if (previousMillis == -1) {
        previousMillis = millis();
      } else {
        tBetweenPulse = millis() - previousMillis;
        previousMillis = millis();
      }

      encoder0Pos--;
    } else {
      if (previousMillis == -1) {
        previousMillis = millis();
      } else {
        tBetweenPulse = millis() - previousMillis;
        previousMillis = millis();
      }
      encoder0Pos++;
    }
  }
  encoder0PinALast = encoderPinVal;
}

// Takes an int between 0-180 that defines where to turn to (0 full left, 180
// full right)
void handleTurn(int poz) {
  // reads the value of the potentiometer (value between 0 and 1023)
  int val = analogRead(potpin);
  val = map(val, 0, 1023, 0, 360); // Map pot values to make sense to servo

  // If the current pot position is not equal to the turn value, turn the motor
  if (poz != val) {
    turnSpeedController.write(poz);
  }
}

// Handles changing the state between EMERGENCY STOP, SYSTEM STOP, and others
void handleStateChange(int newState) {
  switch (newState) {
  case 1:
    handleTurn(360);
    break;

  default:
    break;
  }
}