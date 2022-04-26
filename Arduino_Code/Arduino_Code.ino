int hoek = 0;
int positie = 5;
int inductie = 7;

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(inductie, INPUT);
}

void loop() {


  // Aan de hand van seriele communcatie positie kiezen
  while (Serial.available() == 0) {}
  positie = Serial.parseInt();
  Serial.println(positie);

  if (positie == 1) {
    hoek = 10;
    //potje oppakken
    DC_links();
    inductieSensor(hoek);
    Serial.println("eerste hoek voltooid");
    DC_stop();
    delay(1000);
    //potje neerzetten
    DC_rechts();
    inductieSensor(hoek);
    Serial.println("tweede hoek voltooid");
    DC_stop();
  }
  else if (positie == 2) {
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
  else if (positie == 3) {
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
  else if (positie == 4) {
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
  positie = 5;
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
    Serial.println(digitalRead(inductie));

    while (digitalRead(inductie) == 1) {
    }
    Serial.println(digitalRead(inductie));
          //Serial.println(x);

  }
}

// serial print variable type
void types(String a) { Serial.println("it's a String"); }
void types(int a) { Serial.println("it's an int"); }
void types(char *a) { Serial.println("it's a char*"); }
void types(float a) { Serial.println("it's a float"); }
void types(bool a) { Serial.println("it's a bool"); }
