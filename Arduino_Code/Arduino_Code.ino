#include <Wire.h>

#define WIRE_ID 8
#define BYTE_COUNT 2

int unit = -1;
int choice = -1;

int oldUnit;

boolean manual = false;

int hoek = 0;
int positieInt = 2;
String positieString;
String command;
int inductie = 7;
int calibratieL = A0;
int calibratieR = A1;
int cilinderIn = A3;
int cilinderUit = A2;
int grijperHoog = 5;
int grijperLaag = 6;
int cilinder = 8;
int potDetectieSensor = 9;

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
  pinMode(inductie, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(cilinder, OUTPUT);
  pinMode(potDetectieSensor, INPUT);

  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);

  startProcedure = true;

  digitalWrite(cilinder, LOW);
  if(digitalRead(grijperLaag) == HIGH){
    stepperOmlaag();
  }

  DC_links(speedDC);
  while(analogRead(calibratieL)>450){}
  
  DC_stop(speedDC, false);
  delay(500);
  speedDC = 255;
  DC_rechts(speedDC);
  inductieSensor(6, true);
  DC_stop(speedDC, true);
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
    positieInt = command.toInt();
  
  }else if (command == "CALIBRATE") {
    //Hier alle code voor het kalibreren

    if (analogRead(cilinderIn) < 450 && analogRead(cilinderUit) < 450) {
      Serial.println("CYLINDER_ERROR");
    } else {
      Serial.println("Cilinder schuift uit");
      digitalWrite(cilinder, HIGH);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(analogRead(cilinderUit)) < 450) {
          break;
        }
      }
      if (analogRead(cilinderUit) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }
      Serial.println("Cilinder schuift in");
      digitalWrite(cilinder, LOW);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cilinderUit) < 450) {
          break;
        }
      }
      if (analogRead(cilinderUit) < 450) {
        Serial.println("CYLINDER_CLOSE_ERROR");
      }

    }

    if (digitalRead(grijperHoog) == LOW && digitalRead(grijperLaag) == LOW) {
      Serial.println("STEPPER_ERROR");
    } else if (digitalRead(grijperLaag) == LOW) {
      stepperOmhoog();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(grijperHoog) == LOW) {
          break;
        }
      }
      if (digitalRead(grijperHoog) == HIGH) {
        Serial.println("STEPPER_LIFT_ERROR");
      }
    } else if (digitalRead(grijperHoog) == LOW) {
      stepperOmlaag();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(grijperLaag) == LOW) {
          break;
        }
      }
      if (digitalRead(grijperLaag) == HIGH) {
        Serial.println("STEPPER_DOWN_ERROR");
      }
    } else {
      stepperOmhoog();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(grijperHoog) == LOW) {
          break;
        }
      }
      if (digitalRead(grijperHoog) == HIGH) {
        Serial.println("STEPPER_LIFT_ERROR");
      }

      stepperOmlaag();

      for (int x = 0; x < 100; x++) {
        delay(100);
        if (digitalRead(grijperLaag) == LOW) {
          break;
        }
      }
      if (digitalRead(grijperLaag) == HIGH) {
        Serial.println("STEPPER_DOWN_ERROR");
      }
    }
    stepperOmlaag();

    speedDC = 255;
    if (analogRead(calibratieL) < 450 && analogRead(calibratieR) < 450) {
      Serial.println("DC_ERROR");
    } else if (analogRead(calibratieR) > 450 && analogRead(calibratieL) > 450) {
      DC_rechts(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(calibratieR) < 450) {
          DC_stop(speedDC, true);
          break;
        }
      }
      if (analogRead(calibratieR) > 450) {
        DC_stop(speedDC, true);
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      DC_stop(speedDC, true);

      DC_links(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(calibratieL) < 450) {
          DC_stop(speedDC, false);
          break;
        }
      }
      if (analogRead(calibratieL) > 450) {
        DC_stop(speedDC, false);
        Serial.println("TURN_LEFT_BLOCKED_ERROR");
      }
      DC_stop(speedDC, false);
    } else if (analogRead(calibratieR) > 450) {
      DC_rechts(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(calibratieR) < 450) {
          DC_stop(speedDC, true);
          break;
        }
      }
      if (analogRead(calibratieR) > 450) {
        DC_stop(speedDC, true);
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      DC_stop(speedDC, true);
    } else if (analogRead(calibratieL) > 450) {
      DC_links(speedDC);
      for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(calibratieL) < 450) {
          DC_stop(speedDC, false);
          break;
        }
      }
      if (analogRead(calibratieL) > 450) {
        DC_stop(speedDC, false);
        Serial.println("TURN_RIGHT_BLOCKED_ERROR");
      }
      DC_stop(speedDC, false);
    }
    DC_rechts(speedDC);
    inductieSensor(7, true);
    DC_stop(speedDC, true);
    speedDC = 255;
  
    stopped = true;
    Serial.println("Mode is Kalibreren");
  } else {
    Serial.println("Mode is Handmatig");
    automaticMode = false;
    stopped = false;
    positieInt = command.toInt();
  }

  if (automaticMode && !stopped) {
    Serial.println("I2C start en vraagt naar positie");
    Wire.requestFrom(WIRE_ID, BYTE_COUNT);
    
    if (Wire.available()) {
      unit = Wire.read();
      choice = Wire.read();
      Serial.println("Unit: " + String(unit) + " & Choice: " + String(choice));
      if (unit != oldUnit) {
        positieString = String(choice);
        positieInt = positieString.toInt();
        oldUnit = unit;
      }else{
        delay(500);
      }
    }
  }

  while(digitalRead(potDetectieSensor) == HIGH){}
  delay(1000);

  if (positieInt == 2) {
    hoek = 3;
    potjeOpakken();
    delay(20);
    DC_links(speedDC);
    inductieSensor(hoek, false);
    DC_stop(speedDC, false);
    delay(20);
    potjeNeerzetten();
    delay(20);
    DC_rechts(speedDC);
    inductieSensor(hoek, true);
    DC_stop(speedDC, true);
    Serial.println("2");
  }
  else if (positieInt == 4) {
    hoek = 3;
    potjeOpakken();
    delay(20);
    DC_rechts(speedDC);
    inductieSensor(hoek, true);
    DC_stop(speedDC, true);
    delay(20);
    potjeNeerzetten();
    delay(20);
    DC_links(speedDC);
    inductieSensor(hoek, false);
    DC_stop(speedDC, false);
    Serial.println("4");
  }
  else if (positieInt == 3) {
    hoek = 9;
    potjeOpakken();
    delay(20);
    DC_rechts(speedDC);
    inductieSensor(hoek, true);
    DC_stop(speedDC, true);
    delay(20);
    potjeNeerzetten();
    delay(20);
    DC_links(speedDC);
    inductieSensor(hoek, false);
    DC_stop(speedDC, false);
    Serial.println("3");
  }
  else if (positieInt == 1) {
    hoek = 12;
    potjeOpakken();
    delay(20);
    DC_rechts(speedDC);
    inductieSensor(hoek, true);
    DC_stop(speedDC, true);
    delay(20);
    potjeNeerzetten();
    delay(20);
    DC_links(speedDC);
    inductieSensor(hoek, false);
    DC_stop(speedDC, false);
    Serial.println("1");
  }
  positieInt = 0;
}


// DC motor links omdraaien
void DC_links(int speedOfDC) {
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
void DC_rechts(int speedOfDC) {
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
void DC_stop(int speedOfDC, boolean side) {
  Serial.println("DC_Stop geinitialiseerd");
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
  digitalWrite(cilinder, HIGH);
}

void cylinderIn() {
  Serial.println("cylinderIn geinitialiseerd");
  digitalWrite(cilinder, LOW);
}


// Tellen met inductiesensor
int inductieSensor(int Hoek, boolean side) {
  Serial.println("inductieSensor geinitialiseerd");
  while (digitalRead(inductie) == 1) {}

  for (int x = 0; x < Hoek; x++) { //0 metaal, 1 lucht)
    while (digitalRead(inductie) == 1) {
      if(analogRead(calibratieL)<450 || analogRead(calibratieR)<450){
        Serial.println("DC_ERROR");
        break;
        }
     }
    //Serial.println(digitalRead(inductie));

    while (digitalRead(inductie) == 0) {
      if(analogRead(calibratieL)<450 || analogRead(calibratieR)<450){
        Serial.println("DC_ERROR");
        break;
        }
      }

    if (x == (Hoek - 2)) {
      speedDC = 80;
      if (side) {
        DC_rechts(speedDC);
      } else {
        DC_links(speedDC);
      }
      speedDC = 255;
    }
    //Serial.println(digitalRead(inductie));
    Serial.println("Stap " + String(x) + " van de " + String(Hoek));
  }

  Serial.println("inductieSensor voltooid");
}

//stepper rechtsom
void stepperOmhoog() {
  Serial.println("Stepperomhoog geinitialiseerd");
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  Serial.println("Stepper gaat omhoog");
  int timer = 0;
  while (digitalRead(grijperHoog) == HIGH) {
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
  Serial.println("Stepperomlaag is omhoog");

}

//stepper linksom
void stepperOmlaag() {
  Serial.println("Stepperomlaag geinitialiseerd");
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);

  Serial.println("Stepper gaat omlaag");

  int timer = 0;
  while (digitalRead(grijperLaag) == HIGH) {
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
void potjeOpakken() {
  Serial.println("PotjeOppakken geinitialiseerd");
  stepperOmlaag();

  Serial.println("cilinder op HIGH");
  digitalWrite(cilinder, HIGH);
  
  for (int x = 0; x < 10; x++) {
        delay(100);
        if (analogRead(cilinderIn) < 450) {
          break;
        }
      }
      if (analogRead(cilinderIn) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }

  stepperOmhoog(); //dit kan de verkeerde kant op zijn
  Serial.println("cilinder op LOW");
  digitalWrite(cilinder, LOW);

  for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cilinderUit) < 450) {
          break;
        }
      }
      if (analogRead(cilinderUit) < 450) {
        Serial.println("CYLINDER_CLOSE_ERROR");
      }
  Serial.println("Potje oppakken voltooid");
}

//potje neerzetten
void potjeNeerzetten() {
  Serial.println("PotjeNeerzetten geinitialiseerd");
  Serial.println("cilinder op HIGH");

  digitalWrite(cilinder, HIGH);
  for (int x = 0; x < 10; x++) {
        delay(100);
        if (analogRead(cilinderIn) < 450) {
          break;
        }
      }
      if (analogRead(cilinderIn) < 450) {
        Serial.println("CYLINDER_OPEN_ERROR");
      }

  
  stepperOmlaag();

  Serial.println("cilinder op LOW");
  digitalWrite(cilinder, LOW);
  for (int x = 0; x < 100; x++) {
        delay(100);
        if (analogRead(cilinderUit) < 450) {
          break;
        }
      }
      if (analogRead(cilinderUit) < 450) {
        Serial.println("CYLINDER_CLOSE_ERROR");
      }

  Serial.println("Potje neerzetten voltooid");
}
