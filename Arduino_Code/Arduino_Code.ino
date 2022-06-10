int hoek = 0;
int positie = 5;
int inductie = 7;
int calibiratieL = A0;
int calibritaieR = A1;
int cilinderIn = A2;
int cilinderUit = A3;
int grijperHoog = A4;
int grijperLaag = A5;
int cilinder = 8;

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
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


  // Aan de hand van seriele communcatie positie kiezen
  while (Serial.available() == 0) {}
  positie = Serial.parseInt();
  Serial.println(positie);

//Aan de hand van i2c positie kiezen
//Wire.requestFrom(2, 1);    // request 1 bytes from peripheral device #2
//
//while (Wire.available()) { // peripheral may send less than requested
//  char c = Wire.read(); // receive a byte as character


  if (positie == 1) {
    hoek = 3;
    while(digitalRead(9) == HIGH){}
    potjeOpakken();
    DC_links();
    inductieSensor(hoek);
    DC_stop();
    potjeNeerzetten();
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    Serial.println(0);
  }
  else if (positie == 2) {
    hoek = 3;
    while(digitalRead(9) == HIGH){}
    potjeOpakken();
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    potjeNeerzetten();
    DC_links();
    inductieSensor(hoek);
    DC_stop();
    Serial.println(1);
  }
  else if (positie == 3) {
    hoek = 9;
    while(digitalRead(9) == HIGH){}
    potjeOpakken();
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    potjeNeerzetten();
    DC_links();
    inductieSensor(hoek);
    DC_stop();
    Serial.println(2);
  }
  else if (positie == 4) {
    hoek = 12;
    while(digitalRead(9) == HIGH){}
    potjeOpakken();
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    potjeNeerzetten();
    DC_links();
    inductieSensor(hoek);
    DC_stop();
    Serial.println(3);
  }
  positie = 0;
}


// DC motor links omdraaien
void DC_links() {
  analogWrite(10, 0);
  analogWrite(11, 150);
}

// DC motor rechts omdraaien
void DC_rechts() {
  analogWrite(10, 150);
  analogWrite(11, 0);
}

// DC motor stoppen omdraaien
void DC_stop() {
  analogWrite(10, 0);
  analogWrite(11, 0);
}

// Tellen met inductiesensor
int inductieSensor(int Hoek) {

  for (int x = 0; x < Hoek; x++) { //0 metaal, 1 lucht)
    while (digitalRead(inductie) == 0) {
    }
    //Serial.println(digitalRead(inductie));

    while (digitalRead(inductie) == 1) {
    }
    //Serial.println(digitalRead(inductie));
  }
}

//stepper rechtsom
void stepperOmhoog() {
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  while(analogRead(grijperHoog) > 0){
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  digitalWrite(4, HIGH);

}

//stepper linksom
void stepperOmlaag() {
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);

  while(analogRead(grijperLaag) > 0){
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  digitalWrite(4, HIGH);

}

//potje opakken
void potjeOpakken(){
    stepperOmlaag(); //dit kan de verkeerde kant op zijn
    while(analogRead(cilinderIn) > 0){
      digitalWrite(cilinder, HIGH);
    }
    stepperOmhoog(); //dit kan de verkeerde kant op zijn
    while(analogRead(cilinderUit) > 0){
      digitalWrite(cilinder, LOW);
    }
}

//potje neerzetten
void potjeNeerzetten(){
  while(analogRead(cilinderIn) > 0){
      digitalWrite(cilinder, HIGH);
    }
  stepperOmlaag();
  while(analogRead(cilinderUit) > 0){
      digitalWrite(cilinder, LOW);
    }
}
