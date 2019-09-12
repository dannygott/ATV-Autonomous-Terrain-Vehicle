#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int val;
int encoder0PinA = 32;
int encoder0PinB = 33; 
int encoder0Pos = 1;
int encoder0PinALast = LOW;
int n = LOW;
int servoPoz = 0;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin(2000000);
  myservo.attach(40);
  Serial.setTimeout(10);
  
}

void loop() {
    
    if (Serial.available()){
     // If anything comes in Serial (USB),
     servoPoz = Serial.readStringUntil('!').toInt();
     //bugged, returns to 90, we want it to return to 0
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
  
