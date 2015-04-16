# MAX7219LedMatrix
Library for the ESP8266 on Arduino IDE displaying text on multiple 8x8 led matrices

This library leds you display text and set specific pixels on multiple 8x8 led matrices with a MAX7219 driver chip controlled through the SPI interface.
  These modules are relatively cheep and can be daisy chained which makes it easy to get a led text bar up and running
  You can find modules here: http://www.banggood.com/2Pcs-MAX7219-Dot-Matrix-MCU-LED-Display-Control-Module-Kit-For-Arduino-p-945280.html?p=0P21061109440201501M (<-Affiliate link)
  
  Currently you can set pixels, write text with a simple font and scroll it left or right or have it oscillate between the two ends.

```  
#include <SPI.h>
#include "LedMatrix.h"

LedMatrix ledMatrix;
#define NUMBER_OF_DEVICES 2 // how many devices have you daisy chained?
#define CS_PIN 2 // on which pin the CS slave is connected?

  
void setup() {
  Serial.begin(115200); // For debugging output
  ledMatrix.init(NUMBER_OF_DEVICES, CS_PIN); // Initialize the class

  ledMatrix.setText("MAX7219 Demo"); // set the text
}

void loop() {

  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit(); // commit transfers the byte buffer to the displays
  delay(200);
}
```

# Connecting the modules to your ESP8266
|Led Matrix |	ESP8266                     |
-------------------------------------------
|VCC        |	+3.3V                       |
|GND	      | GND                         |
|DIN	      |GPIO13 (HSPID)               |
|CS	        |Choose free GPIO, e.g. GPIO2 |
|CLK	      |GPIO14 (HSPICLK)             |
