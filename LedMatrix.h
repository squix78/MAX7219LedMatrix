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

    
    void init(byte numberOfDisplays, byte slaveSelectPin);
    void sendByte (const byte device, const byte reg, const byte data);
    void setPixel(byte x, byte y);
    void clear();
    void drawText();
    void setText(String text);
    void setColumn(int column, byte value);
    void commit();
    void scrollTextRight();
    void scrollTextLeft();
    void oscillateText();
    
  private:
    byte cols[16]; // allocate dynamically
    byte spiregister[8];
    byte spidata[8];
    String myText;
    int myTextOffset = 1;
    int increment = -1;
    byte myNumberOfDevices = 0;
    byte mySlaveSelectPin = 0;

};
