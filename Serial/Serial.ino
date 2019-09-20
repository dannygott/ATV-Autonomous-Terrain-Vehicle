#include <Servo.h>
Servo myservo;  // create servo object to control a servo

int encoder0PinA = 32;
int encoder0PinB = 33; 
int encoder0Pos = 0;

int shiftupPin = 24;
int shiftupStop = 25;
int shiftdownPin = 23;
int shiftdownStop = 22;
int encoder0PinALast = LOW;
int servoPoz = 0;

int n = LOW;
int shiftVal = 0;
int prevShiftVal = 0;
int shiftPoz = 1;

int previousMillis = -1;
int tBetweenPulse = 0;
int previousEncoder0Pos = 0;

void setup() {
  pinMode (encoder0PinA, INPUT_PULLUP); //Dont forget to change
  pinMode (encoder0PinB, INPUT_PULLUP);
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
     Serial.println(encoderStopHandler(encoder0Pos));
    }

    handleShift(shiftVal);    
    handleEncoder(digitalRead(encoder0PinA));
    myservo.write(servoPoz);
  }


  int encoderStopHandler(int encoderVal){
    
    if (previousEncoder0Pos == encoderVal){
      tBetweenPulse = 0;
    }
      previousEncoder0Pos = encoder0Pos;
      return tBetweenPulse;
    }

void handleShift(int shiftValue){
  switch (shiftValue) {
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
      digitalWrite(shiftdownStop, HIGH);
      digitalWrite(shiftupStop, HIGH);
      break;
    case 1:
      if(shiftPoz > 0){
        digitalWrite(shiftupStop, LOW);
        digitalWrite(shiftdownPin, HIGH);
        prevShiftVal = shiftValue;
      }
      break;
    
    case 2:
      if(shiftPoz < 3){
        digitalWrite(shiftupStop, LOW);
        digitalWrite(shiftupPin, HIGH);
        prevShiftVal = shiftValue;
      }
      break;
    default:
      break;
  } 
}

void handleEncoder(int encoderPinVal){
  if ((encoder0PinALast == LOW) && (encoderPinVal == HIGH)) {
      
      if (digitalRead(encoder0PinB) == LOW) {
        if (previousMillis == -1){
          previousMillis = millis();
        }
        else{
          tBetweenPulse = millis() - previousMillis;
          previousMillis = millis();
        }
        
        encoder0Pos--;
      } else {
         if (previousMillis == -1){
          previousMillis = millis();
        }
        else{
          tBetweenPulse = millis() - previousMillis;
          previousMillis = millis();
        }
        encoder0Pos++;
        
      }
    }
   encoder0PinALast = encoderPinVal;
}
  
