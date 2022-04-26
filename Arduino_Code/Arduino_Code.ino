int hoek = 0;
int positie = 5;
int inductie = 7;

void setup() {
  Serial.begin(9600);
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


  // Aan de hand van seriele communcatie positie kiezen
  while (Serial.available() == 0) {}
  positie = Serial.parseInt();
  //Serial.println(positie);

  if (positie == 1) {
    hoek = 3;
    //potje oppakken
    DC_links();
    inductieSensor(hoek);
    //Serial.println("eerste hoek voltooid");
    DC_stop();
    stepperRechts();
    stepperLinks();
    //potje neerzetten
    DC_rechts();
    inductieSensor(hoek);
    //Serial.println("tweede hoek voltooid");
    DC_stop();
  }
  else if (positie == 2) {
    hoek = 3;
    //potje oppakken
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();;
    stepperRechts();
    stepperLinks();
    //potje neerzetten
    DC_links();
    inductieSensor(hoek);
    DC_stop();
  }
  else if (positie == 3) {
    hoek = 9;
    //potje oppakken
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    stepperRechts();
    stepperLinks();
    //potje neerzetten
    DC_links();
    inductieSensor(hoek);
    DC_stop();
  }
  else if (positie == 4) {
    hoek = 12;
    //potje oppakken
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    stepperRechts();
    stepperLinks();
    //potje neerzetten
    DC_links();
    inductieSensor(hoek);
    DC_stop();
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
