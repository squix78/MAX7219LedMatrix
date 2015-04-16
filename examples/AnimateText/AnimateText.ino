#include <SPI.h>
#include "LedMatrix.h"

LedMatrix ledMatrix;
#define NUMBER_OF_DEVICES 2
#define CS_PIN 2

  
void setup() {
  ledMatrix.init(NUMBER_OF_DEVICES, CS_PIN);

  ledMatrix.setText("MAX7219 Animation Demo");
}

void loop() {

  ledMatrix.clear();
  ledMatrix.oscillateText();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(200);
}
