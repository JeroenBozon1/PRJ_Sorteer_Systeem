int directionPin;
int stepsPin;

int dcInputOnePin;
int dcInputTwoPin;

int encoder;
int stepperOn;

int stepperButton;

String directionDCInput;
int stepperStepsInput;
int stepperDirectionInput;
int positionPot;

int dcSpeed = 100;

String sensorRotation1 = "A0";
String sensorRotation2 = "A1";

String sensorCylinder1 = "A2";
String sensorCylinder2 = "A3";

String sensorStepper1 = "A4";
String sensorStepper2 = "A5";

int encoderPin = 7;

int lastPosition;

int position0 = 8;
int position1 = 16;
int position2 = 24;
int position3 = 48;
int position4 = 16;
int position5 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);

  pinMode(directionPin,OUTPUT); 
  pinMode(stepsPin,OUTPUT);
   
  pinMode(dcInputOnePin, OUTPUT);
  pinMode(dcInputTwoPin, OUTPUT);

  pinMode(stepperButton, INPUT);

  dcMotor("LEFT");
  while(sensorRotation1==0){}
  dcMotor("OFF");
  
  lastPosition = 5;
}

void loop() {
//hier wordt uitgelezen of er een signaal wordt verstuurd vanuit python
  if (Serial.available() > 0) {
    //positionPot is de integer die door python wordt doorgestuurd
    positionPot = Serial.readString();
    Serial.println(positionPot);
    
    if(positionPot>lastPosition){
      
    }
      encoderArduino(
  }

  if(stepperOn==1){
    stepper(stepperDirectionInput, stepperStepsInput);
  }
}

void encoderArduino(int toPosition, String toDirection){

 int positionCounter = 0;
 
// while(){
//  
// }
}

void stepper(int direction, int steps){
  //Dit is om de stepper aan te zetten, je geeft de kant aan met een int 1 of 0(HIGH of LOW), steps is het aantal stappen.
    digitalWrite(directionPin, direction);

     for(int i = 0; i < steps; i++){
    digitalWrite(stepsPin, HIGH);
    delayMicroseconds(250);
    digitalWrite(stepsPin, LOW);
    delayMicroseconds(250);
  }
}

void dcMotor(String directionDC){
  if(directionDC == "LEFT"){
    analogWrite(dcInputOnePin, dcSpeed);
    analogWrite(dcInputTwoPin, 0);
  }

  if(directionDC == "RIGHT"){
    analogWrite(dcInputOnePin, 0);
    analogWrite(dcInputTwoPin, dcSpeed);
  }

  if(directionDC == "OFF"){
    analogWrite(dcInputOnePin, 0);
    analogWrite(dcInputTwoPin, 0);
  }
}
