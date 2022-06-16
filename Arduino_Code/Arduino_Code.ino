#include <Wire.h>

#define WIRE_ID 8
#define BYTE_COUNT 2

int unit = -1;
int choice = -1;

int oldUnit;

boolean manual = false;

int positionAngle = 0;
int positionInt = 2;
String positionString;
String command;
int induction = 7;
int dcStopSensorLeft = A0;
int dcStopSensorRight = A1;
int cylinderIn = A3;
int cylinderOut = A2;
int gripperUp = 5;
int gripperDown = 6;
int cylinder = 8;
int potDetectionSensor = 9;

int speedDC = 100;
int previousSpeed = 0;
int dcStepSize = 10;

boolean automaticMode = false;
boolean stopped = false;
boolean startProcedure;

void setup() {
  Wire.begin();
  Serial.begin(250000);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(induction, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(cylinder, OUTPUT);
  pinMode(potDetectionSensor, INPUT);

  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);

  startProcedure = true;

  digitalWrite(cylinder, LOW);
  if(digitalRead(gripperDown) == HIGH){
    stepperDown();
  }

  dc_Left(speedDC);
  while(analogRead(dcStopSensorLeft)>450){}
  
  dc_Stop(speedDC, false);
  delay(500);
  speedDC = 255;
  dc_Right(speedDC);
  inductionSensor(6, true);
  dc_Stop(speedDC, true);
  speedDC = 255;

  
}

void loop() {

  if(startProcedure){
    while(Serial.available() == 0){}
    Serial.println("Startprocedure if statement");
    command = Serial.readString();
    startProcedure = false;
  }
  
  if(manual==false) {
    if(Serial.available() > 0){
      Serial.println("automatische if");
      command = Serial.readString();
    }
  }else{
    Serial.println("NEXT");
    while(Serial.available() == 0){}
    Serial.println("Handmatig is true");
    command = Serial.readString();
  }
  
//  Serial.println("commando is: " + command);

  // Aan de hand van seriele communcatie lezen wat de arduino moet doen
  if (command == "AUTO") {
    automaticMode = true;
    stopped = false;
    manual = false;
    Serial.println("Mode is automatisch");

  } else if (command == "STOP") {
    //Hier alle code om hem te stoppen
    stopped = true;
    Serial.println("Mode is STOP");

  }else if (command == "MANUAL"){
    manual = true;
    Serial.println("Handmatige modus");
    automaticMode = false;
    stopped = false;
    while(Serial.available() == 0){}
    command = Serial.readString();
    positionInt = command.toInt();
  
  }else if (command == "CALIBRATE") {
    //Hier alle code voor het kalibreren

    if (analogRead(cylinderIn) < 450 && analogRead(cylinderOut) < 450) {
      Serial.println("CYLINDER_ERROR");
    } else {
      Serial.println("cylinder schuift uit");
      digitalWrite(cylinder, HIGH);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(analogRead(cylinderOut)) < 450) {
          break;
        }
      }
      if (analogRead(cylinderOut) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }
      Serial.println("cylinder schuift in");
      digitalWrite(cylinder, LOW);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cylinderOut) < 450) {
          break;
        }
      }
      if (analogRead(cylinderOut) < 450) {
        Serial.println("CYLINDER_CLOSE_ERROR");
      }

    }

    if (digitalRead(gripperUp) == LOW && digitalRead(gripperDown) == LOW) {
      Serial.println("STEPPER_ERROR");
    } else if (digitalRead(gripperDown) == LOW) {
      stepperUp();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(gripperUp) == LOW) {
          break;
        }
      }
      if (digitalRead(gripperUp) == HIGH) {
        Serial.println("STEPPER_LIFT_ERROR");
      }
    } else if (digitalRead(gripperUp) == LOW) {
      stepperDown();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(gripperDown) == LOW) {
          break;
        }
      }
      if (digitalRead(gripperDown) == HIGH) {
        Serial.println("STEPPER_DOWN_ERROR");
      }
    } else {
      stepperUp();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(gripperUp) == LOW) {
          break;
        }
      }
      if (digitalRead(gripperUp) == HIGH) {
        Serial.println("STEPPER_LIFT_ERROR");
      }

      stepperDown();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(gripperDown) == LOW) {
          break;
        }
      }
      if (digitalRead(gripperDown) == HIGH) {
        Serial.println("STEPPER_DOWN_ERROR");
      }
    }
    stepperDown();

    speedDC = 255;
    if (analogRead(dcStopSensorLeft) < 450 && analogRead(dcStopSensorRight) < 450) {
      Serial.println("DC_ERROR");
    } else if (analogRead(dcStopSensorRight) > 450 && analogRead(dcStopSensorLeft) > 450) {
      dc_Right(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorRight) < 450) {
          dc_Stop(speedDC, true);
          break;
        }
      }
      if (analogRead(dcStopSensorRight) > 450) {
        dc_Stop(speedDC, true);
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      dc_Stop(speedDC, true);

      dc_Left(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorLeft) < 450) {
          dc_Stop(speedDC, false);
          break;
        }
      }
      if (analogRead(dcStopSensorLeft) > 450) {
        dc_Stop(speedDC, false);
        Serial.println("TURN_LEFT_BLOCKED_ERROR");
      }
      dc_Stop(speedDC, false);
    } else if (analogRead(dcStopSensorRight) > 450) {
      dc_Right(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorRight) < 450) {
          dc_Stop(speedDC, true);
          break;
        }
      }
      if (analogRead(dcStopSensorRight) > 450) {
        dc_Stop(speedDC, true);
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      dc_Stop(speedDC, true);
    } else if (analogRead(dcStopSensorLeft) > 450) {
      dc_Left(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorLeft) < 450) {
          dc_Stop(speedDC, false);
          break;
        }
      }
      if (analogRead(dcStopSensorLeft) > 450) {
        dc_Stop(speedDC, false);
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      dc_Stop(speedDC, false);
    }
    dc_Right(speedDC);
    inductionSensor(7, true);
    dc_Stop(speedDC, true);
    speedDC = 255;
  
    stopped = true;
    Serial.println("Mode is Kalibreren");
  } else {
    Serial.println("Mode is Handmatig");
    automaticMode = false;
    stopped = false;
    positionInt = command.toInt();
  }

  if (automaticMode && !stopped) {
    Serial.println("I2C start en vraagt naar positie");
    Wire.requestFrom(WIRE_ID, BYTE_COUNT);
    
    if (Wire.available()) {
      unit = Wire.read();
      choice = Wire.read();
      Serial.println("Unit: " + String(unit) + " & Choice: " + String(choice));
      if (unit != oldUnit) {
        positionString = String(choice);
        positionInt = positionString.toInt();
        oldUnit = unit;
      }else{
        delay(500);
      }
    }
  }

  while(digitalRead(potDetectionSensor) == HIGH){}
  delay(1000);

  if (positionInt == 2) {
    positionAngle = 3;
    potGrab();
    delay(20);
    dc_Left(speedDC);
    inductionSensor(positionAngle, false);
    dc_Stop(speedDC, false);
    delay(20);
    potRelease();
    delay(20);
    dc_Right(speedDC);
    inductionSensor(positionAngle, true);
    dc_Stop(speedDC, true);
    Serial.println("2");
  }
  else if (positionInt == 4) {
    positionAngle = 3;
    potGrab();
    delay(20);
    dc_Right(speedDC);
    inductionSensor(positionAngle, true);
    dc_Stop(speedDC, true);
    delay(20);
    potRelease();
    delay(20);
    dc_Left(speedDC);
    inductionSensor(positionAngle, false);
    dc_Stop(speedDC, false);
    Serial.println("4");
  }
  else if (positionInt == 3) {
    positionAngle = 9;
    potGrab();
    delay(20);
    dc_Right(speedDC);
    inductionSensor(positionAngle, true);
    dc_Stop(speedDC, true);
    delay(20);
    potRelease();
    delay(20);
    dc_Left(speedDC);
    inductionSensor(positionAngle, false);
    dc_Stop(speedDC, false);
    Serial.println("3");
  }
  else if (positionInt == 1) {
    positionAngle = 12;
    potGrab();
    delay(20);
    dc_Right(speedDC);
    inductionSensor(positionAngle, true);
    dc_Stop(speedDC, true);
    delay(20);
    potRelease();
    delay(20);
    dc_Left(speedDC);
    inductionSensor(positionAngle, false);
    dc_Stop(speedDC, false);
    Serial.println("1");
  }
  positionInt = 0;
}


// DC motor links omdraaien
void dc_Left(int speedOfDC) {
  if(previousSpeed>speedOfDC){
    for(int x=0; x<=speedOfDC; x-=dcStepSize){
      analogWrite(10, 0);
      analogWrite(11, x);
      delay(1);
    }
  }else{
    for(int x=0; x<=speedOfDC; x+=dcStepSize){
      analogWrite(10, 0);
      analogWrite(11, x);
      delay(1);
    }
  }
  previousSpeed = speedOfDC;
}

// DC motor rechts omdraaien
void dc_Right(int speedOfDC) {
  if(previousSpeed>speedOfDC){
    for(int x=0; x<=speedOfDC; x-=dcStepSize){
      analogWrite(10, x);
      analogWrite(11, 0);
      delay(1);
    }
  }else{
    for(int x=0; x<=speedOfDC; x+=dcStepSize){
      analogWrite(10, x);
      analogWrite(11, 0);
      delay(1);
    }
  }
  previousSpeed = speedOfDC;
}

// DC motor stoppen omdraaien
void dc_Stop(int speedOfDC, boolean side) {
  Serial.println("dc_Stop geinitialiseerd");
  if(side){
    for(int x=speedOfDC; x>=0; x-=dcStepSize){
      analogWrite(10, x);
      analogWrite(11, 0);
      delay(1);
    }
  }else{
    for(int x=speedOfDC; x>=0; x-=dcStepSize){
      analogWrite(10, 0);
      analogWrite(11, x);
      delay(1);
    }
  }
}

void cylinderOut() {
  Serial.println("cylinderOut geinitialiseerd");
  digitalWrite(cylinder, HIGH);
}

void cylinderIn() {
  Serial.println("cylinderIn geinitialiseerd");
  digitalWrite(cylinder, LOW);
}


// Tellen met inductionsensor
int inductionSensor(int positionAngle, boolean side) {
  Serial.println("inductionSensor geinitialiseerd");
  while (digitalRead(induction) == 1) {}

  for (int x = 0; x < positionAngle; x++) { //0 metaal, 1 lucht)
    while (digitalRead(induction) == 1) {
      if(analogRead(dcStopSensorLeft)<450 || analogRead(dcStopSensorRight)<450){
        Serial.println("DC_ERROR");
        break;
        }
     }
    //Serial.println(digitalRead(induction));

    while (digitalRead(induction) == 0) {
      if(analogRead(dcStopSensorLeft)<450 || analogRead(dcStopSensorRight)<450){
        Serial.println("DC_ERROR");
        break;
        }
      }

    if (x == (positionAngle - 2)) {
      speedDC = 80;
      if (side) {
        dc_Right(speedDC);
      } else {
        dc_Left(speedDC);
      }
      speedDC = 255;
    }
    //Serial.println(digitalRead(induction));
    Serial.println("Stap " + String(x) + " van de " + String(positionAngle));
  }

  Serial.println("inductionSensor voltooid");
}

//stepper rechtsom
void stepperUp() {
  Serial.println("stepperUp geinitialiseerd");
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  Serial.println("Stepper gaat omhoog");
  int timer = 0;
  while (digitalRead(gripperUp) == HIGH) {
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
    timer+=2;
    if(timer == 6000){
      Serial.println("STEPPER_LIFT_ERROR");
      break;
    }
  }
  digitalWrite(4, HIGH);
  Serial.println("stepperDown is omhoog");

}

//stepper linksom
void stepperDown() {
  Serial.println("stepperDown geinitialiseerd");
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);

  Serial.println("Stepper gaat omlaag");

  int timer = 0;
  while (digitalRead(gripperDown) == HIGH) {
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
    timer+=2;
    if(timer == 6000){
      Serial.println("STEPPER_DOWN_ERROR");
      break;
    }
  }
  digitalWrite(4, HIGH);
  Serial.println("Stepper is omlaag");

}

//potje opakken
void potGrab() {
  Serial.println("PotjeOppakken geinitialiseerd");
  stepperDown();

  Serial.println("cylinder op HIGH");
  digitalWrite(cylinder, HIGH);
  
  for (int x = 0; x < 10; x++) {
        delay(100);
        if (analogRead(cylinderIn) < 450) {
          break;
        }
      }
      if (analogRead(cylinderIn) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }

  stepperUp(); //dit kan de verkeerde kant op zijn
  Serial.println("cylinder op LOW");
  digitalWrite(cylinder, LOW);

  for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cylinderOut) < 450) {
          break;
        }
      }
      if (analogRead(cylinderOut) < 450) {
        Serial.println("CYLINDER_CLOSE_ERROR");
      }
  Serial.println("Potje oppakken voltooid");
}

//potje neerzetten
void potRelease() {
  Serial.println("potRelease geinitialiseerd");
  Serial.println("cylinder op HIGH");

  digitalWrite(cylinder, HIGH);
  for (int x = 0; x < 10; x++) {
        delay(100);
        if (analogRead(cylinderIn) < 450) {
          break;
        }
      }
      if (analogRead(cylinderIn) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }

  
  stepperDown();

  Serial.println("cylinder op LOW");
  digitalWrite(cylinder, LOW);
  for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cylinderOut) < 450) {
          break;
        }
      }
      if (analogRead(cylinderOut) < 450) {
        Serial.println("CYLINDER_CLOSE_ERROR");
      }

  Serial.println("Potje neerzetten voltooid");
}
