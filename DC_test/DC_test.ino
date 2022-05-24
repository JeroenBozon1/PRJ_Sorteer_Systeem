void setup() {
  // put your setup code here, to run once:
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);

DC_links();
  delay(500);
  DC_stop();
  delay(500);
  DC_rechts();
  delay(500);
  DC_stop();
  delay(500);


}

void loop() {
  // put your main code here, to run repeatedly:
  //richting veranderen door de nul en de waarde om te draaien

  

  //if (Serial.read()="positie1"){
//  DC_links();
//  delay(100);
//  DC_stop();
//  delay(100);
//  DC_rechts();
//  delay(100);
//  Dc_stop();
  //}
//  else if (Serial.read()="positie2"){
//  DC_rechts();
//  DC_links();
//  }
//  else if (Serial.read()="positie3"){
//  DC_rechts();
//  DC_links();
//  }
//  else if (Serial.read()="positie4"){
//  DC_rechts();
//  DC_links();
//  }

}

void DC_links() {
    analogWrite(10,0);
    analogWrite(11,255);
}

void DC_rechts() {
    analogWrite(10,255);
    analogWrite(11,0);
}

void DC_stop() {
    analogWrite(10,0);
    analogWrite(11,0);
}
