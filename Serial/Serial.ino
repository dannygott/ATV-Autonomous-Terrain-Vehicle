#include <Servo.h>
Servo myservo;  // create servo object to control a servo
void setup() {
  Serial.begin(2000000);
  myservo.attach(40);
  Serial.setTimeout(10);
  
}

void loop() {

    if (Serial.available()){
     // If anything comes in Serial (USB),
     int servoPoz = Serial.readStringUntil('!').toInt();
     myservo.write(servoPoz); 
    }
    else{
      myservo.write(0);                
    }
  }
  
