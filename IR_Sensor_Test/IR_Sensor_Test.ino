int sensorPin = 9;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(sensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(sensorPin));

}
