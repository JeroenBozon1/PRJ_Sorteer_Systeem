//
//
//void setup() {
//  // put your setup code here, to run once:
//  pinMode(2,OUTPUT);
//  pinMode(3,OUTPUT);
//  pinMode(4,OUTPUT);
//  digitalWrite(2, LOW);
//  digitalWrite(3, LOW);
//  digitalWrite(4, HIGH);// HIGH, LOW directie
//
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  digitalWrite(3,HIGH);
//  digitalWrite(3,LOW);
//  delay(1);
//}
/*Example sketch to control a stepper motor with A4988 stepper motor driver, AccelStepper library and Arduino: continuous rotation. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <AccelStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define enablePin 4
#define motorInterfaceType 1


int distance = 500;
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(115200);
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(4000);
  stepper.setAcceleration(100);
  stepper.setSpeed(4000);
}

void loop() {

  stepper.moveTo(distance);

  distance = -distance;

  delay(1000);
  
  // Step the motor with a constant speed as set by setSpeed():
  while (stepper.distanceToGo()) {
    Serial.println(stepper.distanceToGo());
    stepper.run();
    delay(2);
  }

  delay(1000);
}
