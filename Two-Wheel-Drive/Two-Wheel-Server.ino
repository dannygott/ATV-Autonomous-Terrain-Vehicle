#include <Servo.h>

// Servo objects
Servo leftMotor;
Servo rightMotor;
// Indicator Values
// TODO Change indicator pins to actual pins on the board
int lightSolidPin = 0;
int lightBlinkPin = 0;
int honkPin = 0;

int throttlePoz = 0;

int leftMotorValue = 0;
int rightMotorValue = 0;

void setup() {
  Serial.begin(2000000);
  leftMotor.attach(40);
  rightMotor.attach(41);
  Serial.setTimeout(10);
}

void loop() {

  // If anything comes in Serial (USB)
  if (Serial.available()) {
    leftMotorValue = Serial.readStringUntil(',').toInt();
    rightMotorValue = Serial.readStringUntil(',').toInt();
  }
  leftMotor.write(leftMotorValue);
  rightMotor.write(rightMotorValue);
}

