#include <RoboClaw.h>

#include <Servo.h>
#include <SoftwareSerial.h>


// Servo objects
Servo leftMotor;
Servo rightMotor;
// Indicator Values
// TODO Change indicator pins to actual pins on the board
int lightSolidPin = 0;
int lightBlinkPin = 0;
int honkPin = 0;
RoboClaw roboclaw = RoboClaw(&Serial3,10000);
int throttlePoz = 0;

int leftMotorValue = 0;
int rightMotorValue = 0;

void setup() {
  
  roboclaw.begin(38400);
  Serial.begin(2000000);
  leftMotor.attach(3);
  rightMotor.attach(4);
  Serial.setTimeout(10);
  delay(10000);
}

void loop() {

  // If anything comes in Serial (USB)
  if (Serial.available()) {
    
    leftMotorValue = Serial.readStringUntil(',').toInt() + 64;
    rightMotorValue = Serial.readStringUntil(',').toInt() + 64;
    roboclaw.ForwardBackwardM1(0x80,leftMotorValue);
    roboclaw.ForwardBackwardM2(0x80,rightMotorValue);
  }

  //leftMotor.write(leftMotorValue);
  //rightMotor.write(rightMotorValue);
}
