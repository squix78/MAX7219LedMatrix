#include <Arduino.h>

 // max7219 registers
#define MAX7219_REG_NOOP         0x0
#define MAX7219_REG_DIGIT0       0x1
#define MAX7219_REG_DIGIT1       0x2
#define MAX7219_REG_DIGIT2       0x3
#define MAX7219_REG_DIGIT3       0x4
#define MAX7219_REG_DIGIT4       0x5
#define MAX7219_REG_DIGIT5       0x6
#define MAX7219_REG_DIGIT6       0x7
#define MAX7219_REG_DIGIT7       0x8
#define MAX7219_REG_DECODEMODE   0x9
#define MAX7219_REG_INTENSITY    0xA
#define MAX7219_REG_SCANLIMIT    0xB
#define MAX7219_REG_SHUTDOWN     0xC
#define MAX7219_REG_DISPLAYTEST  0xF

class LedMatrix {
  
  public: 

    /**
    * Constructor.
    * numberOfDisplays: number of connected devices
    * slaveSelectPin: CS (or SS) pin connected to your ESP8266
    */
    LedMatrix(byte numberOfDisplays, byte slaveSelectPin);

    /**
    * Initializes the SPI interface
    */
    void init();

    /**
    * Send a byte to a specific device.
    */
    void sendByte (const byte device, const byte reg, const byte data);

    /**
    * Turn on pixel at position (x,y).
    */
    void setPixel(byte x, byte y);

    /**
    * Clear the frame buffer.
    */
    void clear();

    /**
    * Draw the currently set text at the current offset.
    */
    void drawText();

    /**
    * Set the current text.
    */
    void setText(String text);

    /**
    * Set the text that will replace the current text after a complete scroll
    * cycle.
    */
    void setNextText(String nextText);

    /**
    * Set a specific column with a byte value to the framebuffer.
    */
    void setColumn(int column, byte value);

    /**
    * Writes the framebuffer to the displays.
    */
    void commit();

    /**
    * Scroll the text to the right.
    */ 
    void scrollTextRight();

    /**
    * Scroll the text to the left.
    */
    void scrollTextLeft();

    /**
    * Oscilate the text between the two limits.
    */
    void oscillateText();
    
  private:
    byte* cols;
    byte spiregister[8];
    byte spidata[8];
    String myText;
    String myNextText;
    int myTextOffset = 1;
    int increment = -1;
    byte myNumberOfDevices = 0;
    byte mySlaveSelectPin = 0;

};
