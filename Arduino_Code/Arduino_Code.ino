#include <Wire.h>

#define WIRE_ID 8
#define BYTE_COUNT 2

int unit = -1;
int choice = -1;

int hoek = 0;
int positieInt;
String positieString;
String command;
int inductie = 7;
int cilinder = 8;

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

  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);

}

void loop() {


  while (Serial.available() == 0) {}
  command = Serial.readString();

// Aan de hand van seriele communcatie lezen wat de arduino moet doen
  if (command == "AUTO"){
    automaticMode = true;
    stopped = false;
  }else if (command == "STOP"){
    //Hier alle code om hem te stoppen
    stopped = true;
    
  }else if (command == "CALIBRATE"){
    //Hier alle code voor het kalibreren

    stopped = true;
  }else{
    automaticMode = false;
    stopped = false;
    positieInt = command.toInt();
  }

  if (automaticMode && stopped == false){
    Wire.requestFrom(WIRE_ID, BYTE_COUNT);

    if (Wire.available()) {
    unit = Wire.read();
    choice = Wire.read();
    positieString = String(choice);
    positieInt = positieString.toInt();
    
    }
  }

  if (positieInt == 1) {
    hoek = 3;
    //potje oppakken
    DC_links(255);
    inductieSensor(hoek,"L");
    //Serial.println("eerste hoek voltooid");
    DC_stop();
    stepperRechts();
    stepperLinks();
    //potje neerzetten
    DC_rechts(255);
    inductieSensor(hoek,"R");
    //Serial.println("tweede hoek voltooid");
    DC_stop();
  }
  else if (positieInt == 2) {
    hoek = 3;
    //potje oppakken
    DC_rechts(255);
    inductieSensor(hoek,"R");
    DC_stop();;
    stepperRechts();
    //stepperLinks();
    //potje neerzetten
    DC_links(255);
    inductieSensor(hoek,"L");
    DC_stop();
  }
  else if (positieInt == 3) {
    hoek = 9;
    //potje oppakken
    DC_rechts(255);
    inductieSensor(hoek,"R");
    DC_stop();
    stepperRechts();
    //stepperLinks();
    //potje neerzetten
    DC_links(255);
    inductieSensor(hoek,"L");
    DC_stop();
  }
  else if (positieInt == 4) {
    hoek = 12;
    //potje oppakken
    DC_rechts(255);
    inductieSensor(hoek,"R");
    DC_stop();
    stepperRechts();
    //stepperLinks();
    //potje neerzetten
    DC_links(255);
    inductieSensor(hoek,"L");
    DC_stop();
  }
  positieInt = 0;
}


// DC motor links omdraaien
void DC_links(int speedDC) {
  analogWrite(10, 0);
  analogWrite(11, speedDC);
}

// DC motor rechts omdraaien
void DC_rechts(int speedDC) {
  analogWrite(10, speedDC);
  analogWrite(11, 0);
}

// DC motor stoppen omdraaien
void DC_stop() {
  analogWrite(10, 0);
  analogWrite(11, 0);
}

void cylinderOut(){
  digitalWrite(cilinder, HIGH);
}

void cylinderIn(){
  digitalWrite(cilinder, LOW);
}


// Tellen met inductiesensor
int inductieSensor(int Hoek, String Richting) {

  for (int x = 0; x < Hoek; x++) { //0 metaal, 1 lucht)
    if((Hoek-x)<=1){
      if(Richting == "L"){
        DC_links(100);
      }else if(Richting == "R"){
        DC_rechts(100);
      }
    }
    
    while (digitalRead(inductie) == 0) {
    }
    //Serial.println(digitalRead(inductie));

    while (digitalRead(inductie) == 1) {
    }
    //Serial.println(digitalRead(inductie));
  }
}

//stepper rechtsom
void stepperRechts() {
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  //while(analogRead("A1") > 200){
  for (int i = 0; i < 2000; i++) {
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  digitalWrite(4, HIGH);

}

//stepper linksom
void stepperLinks() {
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);

  //while(analogRead("A1") > 200){
  for (int i = 0; i < 2000; i++) {
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  digitalWrite(4, HIGH);

}
