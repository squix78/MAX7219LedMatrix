#include <SPI.h>
#include "LedMatrix.h"


#define NUMBER_OF_DEVICES 4

// Wiring that works with ESP32
#define CS_PIN 15
#define CLK_PIN 14
#define MISO_PIN 2 //we do not use this pin just fill to match constructor
#define MOSI_PIN 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
int x = 0;

void setup() {
  ledMatrix.init();
  ledMatrix.setRotation(true);

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
