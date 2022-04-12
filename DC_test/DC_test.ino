void setup() {
  // put your setup code here, to run once:
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //richting veranderen door de nul en de waarde om te draaien
analogWrite(10,0);
analogWrite(11,255);
}
