int grijperHoog = A4;
int grijperLaag = A5;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  //pinMode(4, OUTPUT); 

  
}

void loop() {
  delay(100);
  Serial.println("Grijper hoog signaal is: " + String(analogRead(grijperHoog)));
  delay(100);
  Serial.println("Grijper laag signaal is: " + String(analogRead(grijperLaag)));
 
}

void stepperOmhoog() {
  digitalWrite(2, LOW);
  //digitalWrite(4, LOW);

  while(analogRead(grijperHoog) > 0){
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  //digitalWrite(4, HIGH);

}

//stepper linksom
void stepperOmlaag() {
  digitalWrite(2, HIGH);
  //digitalWrite(4, LOW);

  while(analogRead(grijperLaag) > 0){
    digitalWrite(3, HIGH);
    delay(1);
    digitalWrite(3, LOW);
    delay(1);
  }
  //digitalWrite(4, HIGH);

}
