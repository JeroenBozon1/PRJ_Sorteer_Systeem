int hoek = 0;
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

}

void loop() {


// Aan de hand van seriele communcatie positie kiezen
  while (Serial.available == 0) {}
  if (Serial.read() = "positie1") {
    hoek = 10;
    //potje oppakken
    DC_links();
    inductieSensor(hoek);
    //potje neerzetten
    DC_rechts();
    inductieSensor(hoek);
  }
  else if (Serial.read() = "positie2") {
    hoek = 15;
    //potje oppakken
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    //potje neerzetten
    DC_links();
    inductieSensor(hoek);
    DC_stop();
  }
  else if (Serial.read() = "positie3") {
    hoek = 20;
    //potje oppakken
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    //potje neerzetten
    DC_links();
    inductieSensor(hoek);
    DC_stop();
  }
  else if (Serial.read() = "positie4") {
    hoek = 25;
    //potje oppakken
    DC_rechts();
    inductieSensor(hoek);
    DC_stop();
    //potje neerzetten
    DC_links();
    inductieSensor(hoek);
    DC_stop();
  }

}
// DC motor links omdraaien
void DC_links() {
  analogWrite(10, 0);
  analogWrite(11, 255);
}

// DC motor rechts omdraaien
void DC_rechts() {
  analogWrite(10, 255);
  analogWrite(11, 0);
}

// DC motor stoppen omdraaien
void DC_stop() {
  analogWrite(10, 0);
  analogWrite(11, 0);
}

// Tellen met inductiesensor
void inductieSensor(hoek) {
  for (int x = 0; x < hoek; x++) { //0 metaal, 1 lucht)
    while (digitalRead(inductie) != 0) {
    }
    Serial.println(digitalRead(inductie));

    while (digitalRead(inductie) != 1) {
    }
    Serial.println(digitalRead(inductie));
  }
}
