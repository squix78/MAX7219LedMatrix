# MAX7219LedMatrix
Library for the ESP8266 on Arduino IDE displaying text on one or multiple MAX7219 8x8 led matrices.

This library displays text and sets specific pixels on one or multiple 8x8 led matrices with a MAX7219 driver chip controlled through the SPI interface.
These modules are relatively cheep and can be daisy chained which makes it easy to get a led text bar up and running
You can find modules e.g. with [Banggood](http://www.banggood.com/2Pcs-MAX7219-Dot-Matrix-MCU-LED-Display-Control-Module-Kit-For-Arduino-p-945280.html?p=0P21061109440201501M) (<-affiliate link).

For details about the MAX7219 theory, wiring, schematic, etc. there's a great post by Nick Gammon: http://www.gammon.com.au/forum/?id=11516 
  
Currently this library supports the following operations:

- set pixels
- write text with a simple font
- scroll text left or right 
- oscillate text between the two ends
  
You're welcome to [read in my blog](http://blog.squix.ch/2015/04/esp8266arduino-max7219-8x8-led-matrix.html) how this library came about.

## Example

```
#include <SPI.h>
#include "LedMatrix.h"

#define NUMBER_OF_DEVICES 1
#define CS_PIN 2
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

void setup() {
  Serial.begin(115200); // For debugging output
  ledMatrix.init();
  ledMatrix.setIntensity(4); // range is 0-15
  ledMatrix.setText("MAX7219 Demo");
}

void loop() {
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit(); // commit transfers the byte buffer to the displays
  delay(200);
}
```
## Installing library in Arduino IDE
- open Arduino IDE
- open preferences and take note of the 'Sketchbook location' path
- navigate into the `libraries` sub folder at that path (e.g. with terminal)
- clone this Git repository into that folder
- restart Arduino IDE
- you should now find the MAX7219LedMatrix library in Sketch > Include Library

## Connecting the module(s) to the ESP8266

|LED Matrix |	ESP8266                     |
|-----------|-----------------------------|
|VCC        |	+3.3V                       |
|GND	      | GND                         |
|DIN	      |GPIO13 (HSPID)               |
|CS	        |Choose free GPIO, e.g. GPIO2 |
|CLK	      |GPIO14 (HSPICLK)             |
