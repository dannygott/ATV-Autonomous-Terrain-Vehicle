#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int val;
int encoder0PinA = 32;
int encoder0PinB = 33; 
int encoder0Pos = 1;
int encoder0PinALast = LOW;
int n = LOW;
int servoPoz = 0;
int shiftupPin = 22;
int shiftdownPin = 23;
int shiftVal = 1;
//1 is neutral

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (shiftupPin, OUTPUT);
  pinMode (shiftdownPin, OUTPUT);
  Serial.begin(2000000);
  myservo.attach(40);
  Serial.setTimeout(10);
  
}

void loop() {
    
    if (Serial.available()){
     // If anything comes in Serial (USB),
     servoPoz = Serial.readStringUntil(',').toInt();
     digitalWrite( shiftupPin,Serial.readStringUntil(',').toInt());
     digitalWrite( shiftdownPin,Serial.readStringUntil(',').toInt());
     Serial.println(encoder0Pos);
    }
  myservo.write(servoPoz); 
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
  }
  
  
  encoder0PinALast = n;


  }
  
