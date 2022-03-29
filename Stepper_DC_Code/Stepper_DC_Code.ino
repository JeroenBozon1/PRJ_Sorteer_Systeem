int directionPin;
int stepsPin;

int dcInputOnePin;
int dcInputTwoPin;

int encoder;
int stepperOn;

int stepperButton;

int directionDCInput;
int stepperStepsInput;
int stepperDirectionInput;
int speedDCInput;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(directionPin,OUTPUT); 
  pinMode(stepsPin,OUTPUT);
   
  pinMode(dcInputOnePin, OUTPUT);
  pinMode(dcInputTwoPin, OUTPUT);

  pinMode(stepperButton, INPUT);
  
}

void loop() {
  // hier moet op de 1 of andere manier steeds naar de output van python gevraagd worden dus dat moeten we effe bekijken
//  read(ietsVanPython) = dcDirectionInput;
//  read(ietsVanPython) = dcSpeedInput;
//  read(ietsVanPython) = encoder;

//  read(ietsVanPython) = stepperStepsInput;
//  read(ietsVanPython) = stepperDirectionInput;
//  read(ietsVanPython) = stepperOn;
//dit hierboven is meer ruige schets van wat er moet gebeuren idk hoe dat moet iig steeds alles ophalen uit python
  
  if(encoder==1){
    dcMotor(dcDirectionInput, dcSpeedInput);
    delay(1000);
    //Dit is om te zorgen dat de plaat gegarandeerd van de knop af is zodat hij niet gelijk stopt doordat de knop nog is ingedrukt
    while(digitalRead encoder = 1){
      delay(1);
      //Encoder moet een 0 doorgeven zodra het systeem moet stoppen.
    }
    dcMotor(0, 0);
  }

  if(stepperOn==1){
    stepper(stepperDirectionInput, stepperStepsInput);
  }
}

void stepper(int direction, int steps){
  //Dit is om de stepper aan te zetten, je geeft de kant aan met een int 1 of 0(HIGH of LOW), steps is het aantal stappen.
    digitalWrite(directionPin, direction);

     for(int i = 0; i < steps; i++){
    digitalWrite(stepsPin, HIGH);
    delayMicroseconds(250);
    digitalWrite(stepsPin, LOW);
    delayMicroseconds(250);
  }
}

void dcMotor(String directionDC, int speedDC){
  if(directionDC == "UP"){
    analogWrite(dcInputOnePin, speedDC);
    analogWrite(dcInputTwoPin, 0);
  }

  if(directionDC == "DOWN"){
    analogWrite(dcInputOnePin, 0);
    analogWrite(dcInputTwoPin, speedDC);
  }
}
