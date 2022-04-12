void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);// HIGH, LOW directie

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3,LOW);
  digitalWrite(3,LOW);
  delay(1);
}
