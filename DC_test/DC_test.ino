void setup() {
  // put your setup code here, to run once:
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //richting veranderen door de nul en de waarde om te draaien


  if (Serial.read()="positie1"){
  DC_links();
  DC_rechts();
  }
  else if (Serial.read()="positie2"){
  DC_rechts();
  DC_links();
  }
  else if (Serial.read()="positie3"){
  DC_rechts();
  DC_links();
  }
  else if (Serial.read()="positie4"){
  DC_rechts();
  DC_links();
  }

}

void DC_links() {
    analogWrite(10,0);
    analogWrite(11,255);
}

void DC_rechts() {
    analogWrite(10,255);
    analogWrite(11,0);
}

void stop() {
    analogWrite(10,0);
    analogWrite(11,0);
}
