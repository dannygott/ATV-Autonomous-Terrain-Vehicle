#include <Servo.h>

// Servo objects
Servo throttleServo;
Servo turnSpeedController;
// Engine Control Values
// TODO Change engine control pins to actual pins on the board
int engineStatorPin = 0;
int engineStartPin = 0;
int engineKillPin = 0;
bool engineRunning = false;
int engineTime = 0;
int engineRPM = 0;
// Indicator Values
// TODO Change indicator pins to actual pins on the board
int lightSolidPin = 0;
int lightBlinkPin = 0;
int honkPin = 0;
// Encoder Values
int encoder0PinA = 32;
int encoder0PinB = 33;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int previousMillis = -1;
int tBetweenPulse = 0;
int previousEncoder0Pos = 0;
// Brake Values
int brakePin = 0; // TODO Change brake value to actual pin on board
bool brakeStatus = 0;
// Shifting Values
int shiftupPin = 24;
int shiftupStop = 25;
int shiftdownPin = 23;
int shiftdownStop = 22;
int shiftVal = 0;
int prevShiftVal = 0;
int shiftPoz = 1;
// Turning Values
// TODO Change turning potentiometer value to actual pin on board
int turnPotPin = 0;
int turnValue = 180;
// Throttle Value
int throttlePoz = 0;
// Indicates status of entire system. 0 = SYSTEMSTOP, 1 = RUNNING, 2 = EMERGENCY
int systemStatus = 0;

void setup() {
  // TODO Change back to old encoder (getrid of INPUT_PULLUP)
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(shiftupPin, OUTPUT);
  pinMode(shiftdownPin, OUTPUT);
  pinMode(brakePin, OUTPUT);
  Serial.begin(2000000);
  throttleServo.attach(40);
  turnSpeedController.attach(41);
  Serial.setTimeout(10);
  init(); // Run all tasks associated with starting the robot
}

void loop() {

  // If anything comes in Serial (USB)
  if (Serial.available()) {

    throttlePoz = Serial.readStringUntil(',').toInt();
    shiftVal = Serial.readStringUntil(',').toInt();
    turnValue = Serial.readStringUntil(',').toInt();
    // systemStatus = Serial.readStringUntil(',').toInt(); TODO Implement Later

    Serial.println(encoder0Pos);
    Serial.println(shiftPoz);
    Serial.println(handleEncoderStop(encoder0Pos));
    // Serial.println(brakeStatus); TODO Implement Later
    // Serial.println(systemStatus); TODO Implement Later
  }
  checkState();
  handleTurn(turnValue);
  handleShift(shiftVal);
  handleEncoder(digitalRead(encoder0PinA));
  digitalwrite(brakePin, brakeStatus);
  throttleServo.write(throttlePoz);
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
  case 2:
    // EMERGENCY STOP
    digitalWrite(lightBlinkPin, HIGH);
    digitalWrite(engineKillPin, HIGH); // Set engine to kill
    handleTurn(360);                   // Set turning to max right
    throttleServo.write(0);            // Return throttle to 0
    digitalWrite(brakePin, HIGH);      // Turn brake on

    // Shift to neutral
    shiftToPoint(0);
    // Check if engine running, Shift to 3
    if (engineRunning == false) {
      shiftToPoint(3);
      handleHonk(1);
    } else {
      handleHonk(2);
    }

    break;
  case 0:
    // TODO Create system stop state
    // SYSTEM STOP
    break;
  default:
    // TODO Create a default condition
    break;
  }
}

// Shift to specified gear
void shiftToPoint(int val) {
  while (shiftPoz != val) {
    if (shiftPoz > val) {
      handleShift(1);
      delay(500); // Delay to allow for return of piston TODO Test this value
      handleShift(0);
      delay(500);
    } else if (shiftPoz < val) {
      handleShift(2);
      delay(500);
      handleShift(0);
      delay(500);
    }
  }
}

// Takes int designating pattern. 1 = Honk 5 times, 2 = Honk on, 0 = Stop
void handleHonk(int pattern) {
  if (pattern == 1) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(honkPin, HIGH);
      delay(500);
      digitalWrite(honkPin, LOW);
      delay(500);
    }
  } else if (pattern == 2) {
    digitalWrite(honkPin, HIGH);
  } else {
    digitalWrite(honkPin, LOW);
  }
}

// Check if systems are running normally. If there is a problem, change the
// state
void checkState() {
  if (engineRPM < 3) {
    engineRunning = false
  }
  if (systemStatus != 1) {
    handleStateChange(systemStatus);
  } else {
    if (engineRunning == false && getRobotSpeed() > 0) {
      // EMERGENCY STOP
      systemStatus = 2;
      handleStateChange(2);
    } else if (engineRunning == false) {
      // SYSTEM STOP
      handleStateChange(0);
    }
  }
}

// TODO Implement getRobotSpeed so we have robot RPM Arduino side
int getRobotSpeed(int pulseTime) {}

int getEngineSpeed() {
  int t = 0;
  if (engineTime > 100000) {
    return 0;
  } else {
    if (digitalread(engineStatorPin) == HIGH) {
      engineTime = millis();
    } else {
      t = millis() - engineTime;
      return t;
    }
  }
}

void init(int t int d) {
  digitalWrite(lightBlinkPin, HIGH);
  digitalWrite(engineStartPin, HIGH);
  delay(t);
  digitalWrite(engineStartPin, LOW);
  delay(d);
  if (engineRPM > 10) {
    handleHonk(1);
    delay(500);
    handleHonk(0);
    digitalWrite(lightBlinkPin, LOW);
    digitalWrite(lightSolidPin, HIGH);
    engineRunning = true;
  } else {
    handleStateChange(0);
  }
}