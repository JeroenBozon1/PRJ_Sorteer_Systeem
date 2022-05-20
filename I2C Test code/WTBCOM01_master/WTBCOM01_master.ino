#include <Wire.h>

#define WIRE_ID 8
#define BYTE_COUNT 2

int unit = -1;
int choice = -1;

void setup() {
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  Wire.requestFrom(WIRE_ID, BYTE_COUNT);

  if (Wire.available()) {
    unit = Wire.read();
    choice = Wire.read();
  }

  Serial.println(String(unit) + "\t|\t" + String(choice));
}
