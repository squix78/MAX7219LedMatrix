#include <SPI.h>
#include "LedMatrix.h"


#define NUMBER_OF_DEVICES 6
#define CS_PIN 2
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);
int x = 0;
  
void setup() {
  ledMatrix.init();

  ledMatrix.setText("MAX7219 Animation Demo");
  ledMatrix.setNextText("Second text");
}

void loop() {

  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(50);
  x=x+1;
  if (x == 400) {
     ledMatrix.setNextText("Third text"); 
  }
}