int cilinder = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(cilinder, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(3000);
  cylinderOut();
  delay(3000);
  cylinderIn();
}

void cylinderOut(){
  digitalWrite(cilinder, HIGH);
  Serial.println("Cilinder op HIGH");
}

void cylinderIn(){
  digitalWrite(cilinder, LOW);
  Serial.println("Cilinder op LOW");
}
