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
int dcStopSensorLeft = 1;
int dcStopSensorRight = 0;
int cylinderInSensor = 2;
int cylinderOutSensor = 3;
int gripperUp = 5;
int gripperDown = 6;
int cylinder = 8;
int potDetectionSensor = 9;

int speedDC = 150;
int previousSpeed = 0;
int dcDelay = 25;
int delayWaitingTime = 100000;

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
  
  while(analogRead(cylinderOutSensor) > 450){}
  Serial.println("Cylinder op uitgeschoven stand");
  
  if(digitalRead(gripperDown) == HIGH){
    stepperDown();
  }

  dc_Left();
  while(analogRead(dcStopSensorLeft)>450){}
  
  dc_Stop();
  delay(200);
  speedDC = 100;
  dc_Right();
  inductionSensor(6, true);
  dc_Stop();
  speedDC = 150;
  delay(200);

  
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

    if (analogRead(cylinderInSensor) < 450 && analogRead(cylinderOutSensor) < 450) {
      Serial.println("CYLINDER_ERROR");
    } else {
      Serial.println("cylinder schuift uit");
      digitalWrite(cylinder, HIGH);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cylinderOutSensor) < 450) {
          break;
        }
      }
      if (analogRead(cylinderOutSensor) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }
      Serial.println("cylinder schuift in");
      digitalWrite(cylinder, LOW);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cylinderOutSensor) < 450) {
          break;
        }
      }
      if (analogRead(cylinderOutSensor) < 450) {
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

    speedDC = 150;
    if (analogRead(dcStopSensorLeft) < 450 && analogRead(dcStopSensorRight) < 450) {
      Serial.println("DC_ERROR");
    } else if (analogRead(dcStopSensorRight) > 450 && analogRead(dcStopSensorLeft) > 450) {
      dc_Right();
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorRight) < 450) {
          dc_Stop();
          break;
        }
      }
      if (analogRead(dcStopSensorRight) > 450) {
        dc_Stop();
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      dc_Stop();
      delay(200);
      dc_Left();
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorLeft) < 450) {
          dc_Stop();
          break;
        }
      }
      if (analogRead(dcStopSensorLeft) > 450) {
        dc_Stop();
        Serial.println("TURN_LEFT_BLOCKED_ERROR");
      }
      dc_Stop();
    } else if (analogRead(dcStopSensorRight) > 450) {
      dc_Right();
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorRight) < 450) {
          dc_Stop();
          break;
        }
      }
      if (analogRead(dcStopSensorRight) > 450) {
        dc_Stop();
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      dc_Stop();
    } else if (analogRead(dcStopSensorLeft) > 450) {
      dc_Left();
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(dcStopSensorLeft) < 450) {
          dc_Stop();
          break;
        }
      }
      if (analogRead(dcStopSensorLeft) > 450) {
        dc_Stop();
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      dc_Stop();
      delay(200);
    }
    dc_Right();
    inductionSensor(7, true);
    dc_Stop();
    delay(200);
    speedDC = 150;
  
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
    dc_Left();
    inductionSensor(positionAngle, false);
    dc_Stop();
    delay(200);
    potRelease();
    delay(20);
    dc_Right();
    inductionSensor(positionAngle, true);
    dc_Stop();
    delay(200);
    Serial.println("2");
  }
  else if (positionInt == 4) {
    positionAngle = 3;
    potGrab();
    delay(20);
    dc_Right();
    inductionSensor(positionAngle, true);
    dc_Stop();
    delay(200);
    potRelease();
    delay(20);
    dc_Left();
    inductionSensor(positionAngle, false);
    dc_Stop();
    delay(200);
    Serial.println("4");
  }
  else if (positionInt == 3) {
    positionAngle = 9;
    potGrab();
    delay(20);
    dc_Right();
    inductionSensor(positionAngle, true);
    delay(dcDelay);
    dc_Stop();
    delay(200);
    potRelease();
    delay(20);
    dc_Left();
    inductionSensor(positionAngle, false);
    dc_Stop();
    delay(200);
    Serial.println("3");
  }
  else if (positionInt == 1) {
    positionAngle = 12;
    potGrab();
    delay(20);
    dc_Right();
    inductionSensor(positionAngle, true);
    delay(dcDelay);
    dc_Stop();
    delay(200);
    potRelease();
    delay(20);
    dc_Left();
    inductionSensor(positionAngle, false);
    dc_Stop();
    delay(200);
    Serial.println("1");
  }
  positionInt = 0;
}


// DC motor links omdraaien
void dc_Left() {
    analogWrite(10,0);
    analogWrite(11,speedDC);
}

// DC motor rechts omdraaien
void dc_Right() {
  analogWrite(10,speedDC);
  analogWrite(11,0);
}

// DC motor stoppen omdraaien
void dc_Stop() {
  Serial.println("dc_Stop geinitialiseerd");
  analogWrite(10, 0);
  analogWrite(11, 0);
}

//void cylinderOut() {
//  Serial.println("cylinderOut geinitialiseerd");
//  digitalWrite(cylinder, HIGH);
//}
//
//void cylinderIn() {
//  Serial.println("cylinderIn geinitialiseerd");
//  digitalWrite(cylinder, LOW);
//}


// Tellen met inductionsensor
int inductionSensor(int positionAngle, boolean side) {
  Serial.println("inductionSensor geinitialiseerd");
  while (digitalRead(induction) == 1) {}

  for (int x = 1; x <= positionAngle; x++) { //0 metaal, 1 lucht)

    while (digitalRead(induction) == 1) {     }
    //Serial.println(digitalRead(induction));

    while (digitalRead(induction) == 0) {}

    if (x == (positionAngle - 2)) {
      speedDC = 70;
      Serial.println("SpeedDC veranderd naar 80");
      if (side) {
        dc_Right();
      } else {
        dc_Left();
      }
      speedDC = 150;
      Serial.println("SpeedDC veranderd naar 150");
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
    if(timer == delayWaitingTime){
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
    timer+=1;
    if(timer == delayWaitingTime){
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
  delay(200);

  Serial.println("cylinder op HIGH");
  digitalWrite(cylinder, HIGH);
  while(analogRead(cylinderInSensor) > 450){}
  delay(200);
  
//  for (int x = 0; x < delayWaitingTime; x++) {
//        delay(10);
//        if (analogRead(cylinderInSensor) < 450) {
//          break;
//        }
//      }
//      if (analogRead(cylinderInSensor) < 450) {
//        Serial.println("CYLINDER_OPEN_ERROR");
//      }
  Serial.println("For loop voorbij");

  stepperUp(); //dit kan de verkeerde kant op zijn
  delay(1000);
  Serial.println("cylinder op LOW");
  digitalWrite(cylinder, LOW);
  while(analogRead(cylinderOutSensor) > 450){}
  delay(1000);

//  for (int x = 0; x < delayWaitingTime; x++) {
//        delay(10);
//        if (analogRead(cylinderOutSensor) < 450) {
//          break;
//        }
//      }
//      if (analogRead(cylinderOutSensor) < 450) {
//        Serial.println("CYLINDER_CLOSE_ERROR");
//      }
//  Serial.println("Potje oppakken voltooid");
}

//potje neerzetten
void potRelease() {
  Serial.println("potRelease geinitialiseerd");
  Serial.println("cylinder op HIGH");

  digitalWrite(cylinder, HIGH);
  while(analogRead(cylinderOutSensor) > 450){}
  
//  for (int x = 0; x < delayWaitingTime; x++) {
//        delay(10);
//        if (analogRead(cylinderInSensor) < 450) {
//          break;
//        }
//      }
//  if (analogRead(cylinderInSensor) < 450) {
//    Serial.println("CYLINDER_OPEN_ERROR");
//  }

  
  stepperDown();
  delay(1000);

  Serial.println("cylinder op LOW");
  digitalWrite(cylinder, LOW);
  while(analogRead(cylinderInSensor) > 450){}
  
//  for (int x = 0; x < delayWaitingTime; x++) {
//        delay(10);
//        if (analogRead(cylinderOutSensor) < 450) {
//          break;
//        }
//      }
//   if (analogRead(cylinderOutSensor) < 450) {
//        Serial.println("CYLINDER_CLOSE_ERROR");
//   }

  Serial.println("Potje neerzetten voltooid");
}
