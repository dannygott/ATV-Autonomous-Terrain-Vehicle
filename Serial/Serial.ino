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
int shiftVal = 0;
int prevShiftVal = 0;
int shiftPoz = 1;
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
     shiftVal = Serial.readStringUntil(',').toInt();
     
     Serial.println(encoder0Pos);
     Serial.println(shiftPoz);
    }

    switch (shiftVal) {
      case 0:
        if(prevShiftVal == 1){
          shiftPoz--;
          prevShiftVal = 0;
        }else if(prevShiftVal == 2){
          shiftPoz++;
          prevShiftVal = 0;
        }
        digitalWrite(shiftupPin, LOW);
        digitalWrite(shiftdownPin, LOW);
        break;
      case 1:
        if(shiftPoz > 0){
          digitalWrite(shiftdownPin, HIGH);
          prevShiftVal = shiftVal;
        }
        break;
      
      case 2:
        if(shiftPoz < 3){
          digitalWrite(shiftupPin, HIGH);
          prevShiftVal = shiftVal;
        }
        break;
      default:
        break;
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
  
