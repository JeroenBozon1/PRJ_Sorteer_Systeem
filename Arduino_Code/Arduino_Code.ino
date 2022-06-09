#include <Wire.h>

#define WIRE_ID 8
#define BYTE_COUNT 2

int unit = -1;
int choice = -1;

int oldUnit;

int hoek = 0;
int positieInt = 2;
String positieString;
String command;
int inductie = 7;
int calibratieL = A0;
int calibratieR = A1;
int cilinderIn = A2;
int cilinderUit = A3;
int grijperHoog = A4;
int grijperLaag = A5;
int cilinder = 8;
int potDetectieSensor = 9;

int speedDC = 100;

boolean automaticMode = false;
boolean stopped = false;

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

  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);

}

void loop() {


  while (Serial.available() == 0) {}
  command = Serial.readString();
  Serial.println("commando is: " + command);

// Aan de hand van seriele communcatie lezen wat de arduino moet doen
  if (command == "AUTO"){
    automaticMode = true;
    stopped = false;
    Serial.println("Mode is automatisch");
    
  }else if (command == "STOP"){
    //Hier alle code om hem te stoppen
    stopped = true;
    Serial.println("Mode is STOP");
    
  }else if (command == "CALIBRATE"){
    //Hier alle code voor het kalibreren

    if(cilinderIn < 450 && cilinderUit < 450){
      Serial.println("CBROKEN");
    }else{
      Serial.println("cilinder op HIGH");
      digitalWrite(cilinder, LOW);
      for (int x=0;x < 100;x++){
        delay(100);
        if(cilinderIn<450){
          break;
          }
        }
        if(cilinderIn>450){
        Serial.println("CYLINDER_OPEN_ERROR");
        }
      digitalWrite(cilinder, HIGH);
      for (int x=0;x < 100;x++){
        delay(100);
        if(cilinderUit<450){
          break;
          }
        }
        if(cilinderUit>450){
        Serial.println("CYLINDER_OPEN_ERROR");
        }
        
      }
      
    if(grijperHoog < 450 && grijperLaag < 450){
      Serial.println("GBROKEN");
    }else if(grijperLaag<450){
      stepperOmhoog();
      
      for (int x=0;x < 100;x++){
        delay(100);
        if(grijperHoog<450){
          break;
          }
        }
       if(grijperHoog>450){
        Serial.println("STEPPER_LIFT_ERROR");
       }
    }else if(grijperHoog<450){
       stepperOmlaag();
      
      for (int x=0;x < 100;x++){
        delay(100);
        if(grijperLaag<450){
          break;
          }
        }
       if(grijperLaag>450){
        Serial.println("STEPPER_DOWN_ERROR");
       }
    }else{
      stepperOmhoog();
      
      for (int x=0;x < 100;x++){
        delay(100);
        if(grijperHoog<450){
          break;
          }
        }
       if(grijperHoog>450){
        Serial.println("STEPPER_LIFT_ERROR");
       }

       stepperOmlaag();
      
      for (int x=0;x < 100;x++){
        delay(100);
        if(grijperLaag<450){
          break;
          }
        }
       if(grijperLaag>450){
        Serial.println("STEPPER_DOWN_ERROR");
       }
    }

    
    if(calibratieL < 450 && calibratieR < 450){
      Serial.println("SBROKEN");
    }else if(calibratieR>450){
      DC_rechts();
      while(calibratieR>450){}
      DC_stop();
    }
    
    
    

    
    stopped = true;
    Serial.println("Mode is Kalibreren");
  }else{
    Serial.println("Mode is Handmatig");
    automaticMode = false;
    stopped = false;
    positieInt = command.toInt();
  }

  if (automaticMode==true && stopped == false){
    Serial.println("I2C start en vraagt naar positie");
    Wire.requestFrom(WIRE_ID, BYTE_COUNT);
    
    if (Wire.available()) {
    unit = Wire.read();
    choice = Wire.read();

      if (unit != oldUnit){
        positieString = String(choice);
        positieInt = positieString.toInt();
        oldUnit = unit;
      }
    }
  }

  if (positieInt == 2) {
    hoek = 3;
    potjeOpakken();
    DC_links();
    inductieSensor(hoek, false);
    DC_stop();
    potjeNeerzetten();
    DC_rechts();
    inductieSensor(hoek+1, true);
    DC_stop();
  }
  else if (positieInt == 4) {
    hoek = 3;
    potjeOpakken();
    DC_rechts();
    inductieSensor(hoek, true);
    DC_stop();
    potjeNeerzetten();
    speedDC = 80;
    DC_links();
    inductieSensor(hoek+1, false);
    DC_stop();
    speedDC = 150;
  }
  else if (positieInt == 3) {
    hoek = 9;
    potjeOpakken();
    DC_rechts();
    inductieSensor(hoek, true);
    DC_stop();
    potjeNeerzetten();
    DC_links();
    inductieSensor(hoek+1, false);
    DC_stop();
  }
  else if (positieInt == 1) {
    hoek = 12;
    potjeOpakken();
    DC_rechts();
    inductieSensor(hoek, true);
    DC_stop();
    potjeNeerzetten();
    DC_links();
    inductieSensor(hoek+1, false);
    DC_stop();
  }
  positieInt = 0;
}


// DC motor links omdraaien
void DC_links() {
  Serial.println("DC_Links geinitialiseerd");
  analogWrite(10, 0);
  analogWrite(11, speedDC);
}

// DC motor rechts omdraaien
void DC_rechts() {
  Serial.println("DC_Rechts geinitialiseerd");
  analogWrite(10, speedDC);
  analogWrite(11, 0);
}

// DC motor stoppen omdraaien
void DC_stop() {
  Serial.println("DC_Stop geinitialiseerd");
  analogWrite(10, 0);
  analogWrite(11, 0);
}

void cylinderOut(){
  Serial.println("cylinderOut geinitialiseerd");
  digitalWrite(cilinder, HIGH);
}

void cylinderIn(){
  Serial.println("cylinderIn geinitialiseerd");
  digitalWrite(cilinder, LOW);
}


// Tellen met inductiesensor
int inductieSensor(int Hoek, boolean side) {
  Serial.println("inductieSensor geinitialiseerd");
  while (digitalRead(inductie) == 1) {}
  
  for (int x = 0; x < Hoek; x++) { //0 metaal, 1 lucht)
    while (digitalRead(inductie) == 0) {}
//Serial.println(digitalRead(inductie));

    while (digitalRead(inductie) == 1) {}

    if(x == (Hoek-2)){
      speedDC = 70;
      if(side){
        DC_rechts();
      }else{
        DC_links();
      }
      speedDC = 150;
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

  Serial.println("Stepperomlaag gaat omhoog");
  while(analogRead(grijperHoog) > 450){
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
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
  while(analogRead(grijperLaag) > 450){
    Serial.println("Bezig met omlaag gaan...");
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  digitalWrite(4, HIGH);
  Serial.println("Stepper is omlaag");

}

//potje opakken
void potjeOpakken(){
    Serial.println("PotjeOppakken geinitialiseerd");
    stepperOmlaag(); //dit kan de verkeerde kant op zijn

    Serial.println("cilinder op HIGH");
    digitalWrite(cilinder, HIGH);
    while(analogRead(cilinderIn) > 450){}
    
    stepperOmhoog(); //dit kan de verkeerde kant op zijn
    Serial.println("cilinder op LOW");
    digitalWrite(cilinder, LOW);
    
    while(analogRead(cilinderUit) > 450){}
    Serial.println("Potje oppakken voltooid");
}

//potje neerzetten
void potjeNeerzetten(){
  Serial.println("PotjeNeerzetten geinitialiseerd");
  Serial.println("cilinder op HIGH");
  
  digitalWrite(cilinder, HIGH);
  while(analogRead(cilinderIn) > 450){}
  stepperOmlaag();

  Serial.println("cilinder op LOW");
  digitalWrite(cilinder, LOW);
  while(analogRead(cilinderUit) > 450){}
  
  Serial.println("Potje neerzetten voltooid");
}
