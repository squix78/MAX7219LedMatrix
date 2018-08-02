#include <SPI.h>
#include "LedMatrix.h"
#include "cp437font.h"

/**
 * Heavily influenced by the code and the blog posts from https://github.com/nickgammon/MAX7219_Dot_Matrix
 */
LedMatrix::LedMatrix(byte numberOfDevices, byte slaveSelectPin) {
        myNumberOfDevices = numberOfDevices;
        mySlaveSelectPin = slaveSelectPin;
        cols = new byte[numberOfDevices * 8];
        xcols = new byte[numberOfDevices * 8];
}

/**
 *  numberOfDevices: how many modules are daisy changed togehter
 *  slaveSelectPin: which pin is controlling the CS/SS pin of the first module?
 */
void LedMatrix::init() {
        pinMode(mySlaveSelectPin, OUTPUT);

        SPI.begin ();
        SPI.setDataMode(SPI_MODE0);
        SPI.setClockDivider(SPI_CLOCK_DIV128);
        for(byte device = 0; device < myNumberOfDevices; device++) {
                sendByte (device, MAX7219_REG_SCANLIMIT, 7); // show all 8 digits
                sendByte (device, MAX7219_REG_DECODEMODE, 0); // using an led matrix (not digits)
                sendByte (device, MAX7219_REG_DISPLAYTEST, 0); // no display test
                sendByte (device, MAX7219_REG_INTENSITY, 0); // character intensity: range: 0 to 15
                sendByte (device, MAX7219_REG_SHUTDOWN, 1); // not in shutdown mode (ie. start it up)
        }
}

void LedMatrix::sendByte (const byte device, const byte reg, const byte data) {
        int offset=device;
        int maxbytes=myNumberOfDevices;

        for(int i=0; i<maxbytes; i++) {
                spidata[i] = (byte)0;
                spiregister[i] = (byte)0;
        }
// put our device data into the array
        spiregister[offset] = reg;
        spidata[offset] = data;

        // enable the line
        digitalWrite(mySlaveSelectPin,LOW);
        // now shift out the data
        for(int i=0; i<myNumberOfDevices; i++) {
                SPI.transfer (spiregister[i]);
                SPI.transfer (spidata[i]);
        }
        digitalWrite (mySlaveSelectPin, HIGH);
}

void LedMatrix::sendByte (const byte reg, const byte data) {
        for(byte device = 0; device < myNumberOfDevices; device++) {
                sendByte(device, reg, data);
        }
}

void LedMatrix::setIntensity(const byte intensity) {
        sendByte(MAX7219_REG_INTENSITY, intensity);
}

void LedMatrix::setCharWidth(byte charWidth) {
        myCharWidth = charWidth;
}

void LedMatrix::setTextAlignment(byte textAlignment) {
        myTextAlignment = textAlignment;
        calculateTextAlignmentOffset();
}

void LedMatrix::calculateTextAlignmentOffset() {
        switch(myTextAlignment) {
        case TEXT_ALIGN_LEFT:
                myTextAlignmentOffset = 0;
                break;
        case TEXT_ALIGN_LEFT_END:
                myTextAlignmentOffset = myNumberOfDevices * 8;
                break;
        case TEXT_ALIGN_RIGHT:
                myTextAlignmentOffset = myText.length() * myCharWidth - myNumberOfDevices * 8;
                break;
        case TEXT_ALIGN_RIGHT_END:
                myTextAlignmentOffset = -myText.length() * myCharWidth;
                break;
        }

}

void LedMatrix::clear() {
        for (byte col = 0; col < myNumberOfDevices * 8; col++) {
                cols[col] = 0;
        }

}

void LedMatrix::commit() {
        // for (byte col = 0; col < myNumberOfDevices * 8; col++) {
        //         sendByte(col / 8, col % 8 + 1, cols[col]);
        // }

        byte index = B0000001;
        byte col;
        //byte xcols[];

        if(deviceOrientation == 0) {
                for (col = 0; col < myNumberOfDevices * 8; col++) {
                        sendByte(col / 8, col % 8 + 1, cols[col]);
                }
        }
        else if(deviceOrientation == 1 ) {    // orient the device vertically
                for (col = 0; col < myNumberOfDevices * 8; col++) {
                        xcols[col] = 0;
                }
                // little inefficient, can be enhanced, rotate the matrix !
                for (col = 0; col < myNumberOfDevices * 8; col++) {
                        for(byte bits = 0; bits < 8; bits++) {
                                xcols[col] |= ((cols[bits + 8*(col/8)] & (index << (col%8))) ?
                                               (B10000000 >> bits) : 0);
                        }
                        sendByte(col / 8, col % 8 + 1, xcols[col]);
                }
        }
}

void LedMatrix::setText(String text) {
        myText = text;
        myTextOffset = 0;
        calculateTextAlignmentOffset();
}

void LedMatrix::setNextText(String nextText) {
        myNextText = nextText;
}

void LedMatrix::scrollTextRight() {
        myTextOffset = (myTextOffset + 1) % ((int)myText.length() * myCharWidth - 5);
}

void LedMatrix::scrollTextLeft() {
        myTextOffset = (myTextOffset - 1) % ((int)myText.length() * myCharWidth + myNumberOfDevices * 8);
        if (myTextOffset == 0 && myNextText.length() > 0) {
                myText = myNextText;
                myNextText = "";
                calculateTextAlignmentOffset();
        }
}

void LedMatrix::oscillateText() {
        int maxColumns = (int)myText.length() * myCharWidth;
        int maxDisplayColumns = myNumberOfDevices * 8;
        if (maxDisplayColumns > maxColumns) {
                return;
        }
        if (myTextOffset - maxDisplayColumns == -maxColumns) {
                increment = 1;
        }
        if (myTextOffset == 0) {
                increment = -1;
        }
        myTextOffset += increment;
}

void LedMatrix::drawText() {
        char letter;
        int position = 0;
        for (int i = 0; i < myText.length(); i++) {
                letter = myText.charAt(i);
                for (byte col = 0; col < 8; col++) {
                        position = i * myCharWidth + col + myTextOffset + myTextAlignmentOffset;
                        if (position >= 0 && position < myNumberOfDevices * 8) {
                                if (flip==0) {
                                        setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
                                }
                                else {
                                        // flip char (byte)
                                        byte x = pgm_read_byte (&cp437_font [letter] [col]);
                                        // byte x = data;
                                        x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
                                        x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
                                        x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
                                        setColumn(position, x);
                                }
                        }
                }
        }
}

void LedMatrix::setColumn(int column, byte value) {
        if (column < 0 || column >= myNumberOfDevices * 8) {
                return;
        }
        cols[column] = value;
}

void LedMatrix::setPixel(byte x, byte y) {
        bitWrite(cols[x], y, true);
}

bool LedMatrix::scrollEnd(){
        int maxColumns = (int)myText.length() * myCharWidth;
        byte maxDisplayColumns = myNumberOfDevices * 8;
        if (myTextOffset == 0) return 1;

        else return 0;
}
