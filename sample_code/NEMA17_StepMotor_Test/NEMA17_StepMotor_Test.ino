#include <AccelStepper.h>

// Step and direction pin numbers
const int stepPin = 23;
const int dirPin = 22;

// Create stepper instance in DRIVER mode (step/dir)
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// Motion control
const int maxSpeed = 1000;
const int acceleration = 1000;
int direction = 0;         // -1 = left, 1 = right, 0 = stop
bool moving = false;       // Whether we're in continuous motion mode
bool goingHome = false;    // Whether we're returning to zero

// Target/home position
const long homePosition = 0;
char key = 's'; // default state is stop

void setup() {
  Serial.begin(9600);

  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(acceleration);
  stepper.setMinPulseWidth(2); // improves reliability at high speed
}

void loop() {
  // ---- Handle Serial Input ----
  if (Serial.available() > 0) {
    key = Serial.read();
    while (Serial.available()) Serial.read(); // flush newline, etc.

    // Handle key actions
    if (key == 'a') {
      direction = -1;
      moving = true;
      goingHome = false;
      Serial.println("Moving left continuously");
    } else if (key == 'd') {
      direction = 1;
      moving = true;
      goingHome = false;
      Serial.println("Moving right continuously");
    } else if (key == 's') {
      direction = 0;
      moving = false;
      goingHome = false;
      stepper.setSpeed(0); // stop immediately if in runSpeed mode
      Serial.println("Stopped");
    } else if (key == 'w') {
      moving = false;
      goingHome = true;
      stepper.moveTo(homePosition);
      Serial.println("Returning to neutral (position 0)");
    }
  }

  // ---- Motion Logic ----
  if (moving) {
    stepper.setSpeed(direction * maxSpeed);
    stepper.runSpeed(); // move indefinitely at constant speed
  } else if (goingHome) {
    if (stepper.currentPosition() != stepper.targetPosition()) {
      stepper.run(); // move to zero with acceleration
    } else {
      goingHome = false;
      Serial.println("Reached neutral position");
    }
  } else {
    // idle or decelerating after stop
    stepper.run(); // needed to finish stop() gracefully
  }
}
