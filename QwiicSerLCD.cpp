/*
 * LCD library for Sparkfun RGB 3.3v Serial Open LCD display
 * with an attached Qwiic adapter.
 *
 * By: Gaston R. Williams
 * Date: August 22, 2018
 *
 * License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 *

 * This library is based heavily on the LiquidCrystal_I2C library and the sample code provided with
 * the Sparkfun Serial OpenLCD display.  The original LiquidCrystal library by David A. Mellis and
 * modified by Limor Fried and the OpenLCD code by Nathan Seidle at Sparkfun.
 *
 * The LiquidCrystal_I2C library was based on the work by DFRobot.
 * (That's the only attribution I found in the code I have. If anyone can provide better information,
 * Plese let me know and I'll be happy to give credit where credit is due.)
 *
 * Original information copied from OpenLCD:
 *
 * The OpenLCD display information is based based on code by
 * Nathan Seidle
 * SparkFun Electronics
 * Date: April 19th, 2015
 *
 * License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 * OpenLCD gives the user multiple interfaces (serial, I2C, and SPI) to control an LCD. SerLCD was the original
 * serial LCD from SparkFun that ran on the PIC 16F88 with only a serial interface and limited feature set.
 * This is an updated serial LCD.
 *
 * Please Note: 0x72 is the 7-bit I2C address. If you are using a different language than Arduino you will probably
 * need to add the Read/Write bit to the end of the address. This means the default read address for the OpenLCD
 * is 0b.1110.0101 or 0xE5 and the write address is 0b.1110.0100 or 0xE4.
 * For more information see https://learn.sparkfun.com/tutorials/i2c
 * Note: This code expects the display to be listening at the default I2C address. If your display is not at 0x72, you can
 * do a hardware reset. Tie the RX pin to ground and power up OpenLCD. You should see the splash screen
 * then "System reset Power cycle me" and the backlight will begin to blink. Now power down OpenLCD and remove
 * the RX/GND jumper. OpenLCD is now reset.
 *
 * To get this code to work, attach a Qwiic adapter to an OpenLCD. Use the Qwiic cable to attach adapter to a Sparkfun Blackboard or
 * an Arduino Uno with the Qwiic shield.
 *
 * The OpenLCD has 4.7k pull up resistors on SDA and SCL. If you have other devices on the
 * I2C bus then you may want to disable the pull up resistors by clearing the PU (pull up) jumper.

 * OpenLCD will work at 400kHz Fast I2C. Use the .setClock() call shown below to set the data rate
 * faster if needed.
 * Command cheat sheet:
 * ASCII / DEC / HEX
 * '|'    / 124 / 0x7C - Put into setting mode
 * Ctrl+c / 3 / 0x03 - Change width to 20
 * Ctrl+d / 4 / 0x04 - Change width to 16
 * Ctrl+e / 5 / 0x05 - Change lines to 4
 * Ctrl+f / 6 / 0x06 - Change lines to 2
 * Ctrl+g / 7 / 0x07 - Change lines to 1
 * Ctrl+h / 8 / 0x08 - Software reset of the system
 * Ctrl+i / 9 / 0x09 - Enable/disable splash screen
 * Ctrl+j / 10 / 0x0A - Save currently displayed text as splash
 * Ctrl+k / 11 / 0x0B - Change baud to 2400bps
 * Ctrl+l / 12 / 0x0C - Change baud to 4800bps
 * Ctrl+m / 13 / 0x0D - Change baud to 9600bps
 * Ctrl+n / 14 / 0x0E - Change baud to 14400bps
 * Ctrl+o / 15 / 0x0F - Change baud to 19200bps
 * Ctrl+p / 16 / 0x10 - Change baud to 38400bps
 * Ctrl+q / 17 / 0x11 - Change baud to 57600bps
 * Ctrl+r / 18 / 0x12 - Change baud to 115200bps
 * Ctrl+s / 19 / 0x13 - Change baud to 230400bps
 * Ctrl+t / 20 / 0x14 - Change baud to 460800bps
 * Ctrl+u / 21 / 0x15 - Change baud to 921600bps
 * Ctrl+v / 22 / 0x16 - Change baud to 1000000bps
 * Ctrl+w / 23 / 0x17 - Change baud to 1200bps
 * Ctrl+x / 24 / 0x18 - Change the contrast. Follow Ctrl+x with number 0 to 255. 120 is default.
 * Ctrl+y / 25 / 0x19 - Change the TWI address. Follow Ctrl+x with number 0 to 255. 114 (0x72) is default.
 * Ctrl+z / 26 / 0x1A - Enable/disable ignore RX pin on startup (ignore emergency reset)
 * '-'    / 45 / 0x2D - Clear display. Move cursor to home position.
 *        / 128-157 / 0x80-0x9D - Set the primary backlight brightness. 128 = Off, 157 = 100%.
 *        / 158-187 / 0x9E-0xBB - Set the green backlight brightness. 158 = Off, 187 = 100%.
 *        / 188-217 / 0xBC-0xD9 - Set the blue backlight brightness. 188 = Off, 217 = 100%.
 *
 * For example, to change the baud rate to 115200 send 124 followed by 18.
 *
 */
#include "QwiicSerLCD.h"

//<<constructor>> setup using default i2c address (0x72)
QwiicSerLCD::QwiicSerLCD(){
}

//<<destructor>>
QwiicSerLCD::~QwiicSerLCD(){/*nothing to destruct*/}

/*
 * Set up the i2c communication with the SerLCD.
 * wirePort - TwoWire port
 * ic2_addr - I2C address
 */
void QwiicSerLCD::begin(TwoWire &wirePort, byte i2c_addr) {
  _i2cAddr 		= i2c_addr;

  begin(wirePort);
} // begin

/*
 * Set up the i2c communication with the SerLCD.
 */
void QwiicSerLCD::begin(TwoWire &wirePort) {
  _i2cPort = &wirePort; //Grab which port the user wants us to use

  //We expect caller to begin their I2C port, with the speed of their choice external to the library
  //But if they forget, we start the hardware here.
  _i2cPort->begin();

  //Call init function since display may have been left in unknown state
  init();
} // begin

/*
 * Initialize the display
 *
 */
void QwiicSerLCD::init() {
   _i2cPort->beginTransmission(_i2cAddr); // transmit to device #1
   _i2cPort->write(SPECIAL_COMMAND); //Send special command character
   _i2cPort->write(LCD_DISPLAYCONTROL | _displayControl); //Send the display command
   _i2cPort->write(SPECIAL_COMMAND); //Send special command character
   _i2cPort->write(LCD_ENTRYMODESET | _displayMode); //Send the entry mode command
   _i2cPort->write(SETTING_COMMAND); //Put LCD into setting mode
   _i2cPort->write(CLEAR_COMMAND); //Send clear display command
   _i2cPort->endTransmission(); //Stop I2C transmission
   delay(10);
 } //init
 /*
  * Send a command to the display.
  * Used by other functions.
  *
  * byte command to send
  */
 void QwiicSerLCD::command(byte command) {
   _i2cPort->beginTransmission(_i2cAddr); // transmit to device
   _i2cPort->write(SETTING_COMMAND); //Put LCD into setting mode
   _i2cPort->write(command); //Send the command code
   _i2cPort->endTransmission(); //Stop I2C transmission

   delay(10); //Hang out for a bit
}

/*
 * Send a special command to the display.  Used by other functions.
 *
 * byte command to send
 */
void QwiicSerLCD::specialCommand(byte command) {
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device
  _i2cPort->write(SPECIAL_COMMAND); //Send special command character
  _i2cPort->write(command); //Send the command code
  _i2cPort->endTransmission(); //Stop I2C transmission

  delay(10); //Hang out for a bit
}
/*
 * Send multiple special commands to the display.
 * Used by other functions.
 *
 * byte command to send
 * byte count number of times to send
 */
void QwiicSerLCD::specialCommand(byte command, byte count) {
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device

  for (int i = 0; i < count; i++) {
    _i2cPort->write(SPECIAL_COMMAND); //Send special command character
    _i2cPort->write(command); //Send command code
  } // for
  _i2cPort->endTransmission(); //Stop I2C transmission

  delay(10); //Hang out for a bit
}

/*
 * Send the clear command to the display.  This clears the
 * display and forces the cursor to return to the beginning
 * of the display.
 */
void QwiicSerLCD::clear() {
  command(CLEAR_COMMAND);
  delay(10);  // a little extra delay after clear
}
/*
 * Send the home command to the display.  This returns the cursor
 * to return to the beginning of the display, without clearing
 * the display.
 */
void QwiicSerLCD::home() {
 specialCommand(LCD_RETURNHOME);
}
/*
 * Set the cursor position to a particular column and row.
 *
 * column - byte 0 to 19
 * row - byte 0 to 3
 *
 * returns: boolean true if cursor set.
 */
void QwiicSerLCD::setCursor(byte col, byte row) {
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  //kepp variables in bounds
  row = max(0, row);      //row cannot be less than 0
  row = min(row, MAX_ROWS-1); //row cannot be greater than max rows

  //send the command
  specialCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
} // setCursor

/*
 * Create a customer character
 * byte   location - character number 0 to 7
 * byte[] charmap  - byte array for character
 */
void QwiicSerLCD::createChar(byte location, byte charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  _i2cPort->beginTransmission(_i2cAddr);
  //Semd request to create a customer character
  _i2cPort->write(SETTING_COMMAND); //Put LCD into setting mode
  _i2cPort->write(27 + location);
  for (int i = 0; i < 8; i++) {
    _i2cPort->write(charmap[i]);
  } // for
  _i2cPort->endTransmission();
  delay(10);
}
/*
 * Write a customer character to the display
 *
 * byte location - character number 0 to 7
 */
void QwiicSerLCD::writeChar(byte location) {
  location &= 0x7; // we only have 8 locations 0-7

  command(35 + location);
}

/*
 * Write a byte to the display.
 * Required for Print.
 */
size_t QwiicSerLCD::write(uint8_t b) {
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device
  _i2cPort->write(b);
  _i2cPort->endTransmission(); //Stop I2C transmission
  delay(10); // wait a bit
 } // write
/*
 * Write a character buffer to the display.
 * Required for Print.
 */
size_t QwiicSerLCD::write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device
	while (size--) {
	  _i2cPort->write(*buffer++);
	  n++;
	} //while
  _i2cPort->endTransmission(); //Stop I2C transmission
  delay(10); //
  return n;
} //write
/*
 * Write a string to the display.
 * Required for Print.
 */
 size_t QwiicSerLCD::write(const char *str) {
   if (str == NULL) return 0;
   return write((const uint8_t *)str, strlen(str));
}

 /*
  * Turn the display off quickly.
  */
 void QwiicSerLCD::noDisplay(){
  _displayControl &= ~LCD_DISPLAYON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
 } // noDisplay

/*
 * Turn the display on quickly.
 */
void QwiicSerLCD::display() {
  _displayControl |= LCD_DISPLAYON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
 } // display
 /*
  * Turn the underline cursor off.
  */
 void QwiicSerLCD::noCursor(){
  _displayControl &= ~LCD_CURSORON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
 } // noCursor

/*
 * Turn the underline cursor on.
 */
void QwiicSerLCD::cursor() {
  _displayControl |= LCD_CURSORON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
 } // cursor

 /*
  * Turn the blink cursor off.
  */
 void QwiicSerLCD::noBlink(){
  _displayControl &= ~LCD_BLINKON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
 } // noBlink

/*
 * Turn the blink cursor on.
 */
void QwiicSerLCD::blink() {
  _displayControl |= LCD_BLINKON;
  specialCommand(LCD_DISPLAYCONTROL | _displayControl);
 } // blink

/*
 * Scroll the display one character to the left, without
 * changing the text
 */
void QwiicSerLCD::scrollDisplayLeft() {
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
 } // scrollDisplayLeft

/*
 * Scroll the display multiple characters to the left, without
 * changing the text
 *
 * count byte - number of characters to scroll
 */
void QwiicSerLCD::scrollDisplayLeft(byte count) {
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, count);
 } // scrollDisplayLeft

/*
 * Scroll the display one character to the right, without
 * changing the text
 */
void QwiicSerLCD::scrollDisplayRight() {
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
 } // scrollDisplayRight

/*
 * Scroll the display multiple characters to the right, without
 * changing the text
 *
 * count byte - number of characters to scroll
 */
void QwiicSerLCD::scrollDisplayRight(byte count) {
  specialCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, count);
 } // scrollDisplayRight

/*
 *  Move the cursor one character to the left.
 */
void QwiicSerLCD::moveCursorLeft() {
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
} // moveCursorLeft

/*
 *  Move the cursor multiple characters to the left.
 *
 *  count byte - number of characters to move
 */
void QwiicSerLCD::moveCursorLeft(byte count) {
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT, count);
} // moveCursorLeft

/*
 *  Move the cursor one character to the right.
 */
void QwiicSerLCD::moveCursorRight() {
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
} // moveCursorRight

/*
 *  Move the cursor multiple characters to the right.
 *
 *  count byte - number of characters to move
 */
void QwiicSerLCD::moveCursorRight(byte count) {
  specialCommand(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT, count);
} // moveCursorRight



/*
 * Use a standard hex rgb value (0x00000000 to 0x00FFFFFF) to set
 * the backlight color.
 *
 * The encoded long value has form (0x00RRGGBB) where RR, GG and BB
 * are red, green, blue byte values in hex.  The remaining two most
 * significant bytes of the long value are ignored.
 *
 * rgb - unsigned long hex encoded rgb value.
 */
void QwiicSerLCD::setBacklight(unsigned long rgb) {
  // convert from hex triplet to byte values
  byte r = (rgb >> 16) & 0x0000FF;
  byte g = (rgb >> 8) & 0x0000FF;
  byte b = rgb & 0x0000FF;

 setBacklight(r, g, b);
}

/*
 * Uses a standard rgb byte triplit eg. (255, 0, 255) to
 * set the backlight color.
 */
void QwiicSerLCD::setBacklight(byte r, byte g, byte b) {
  // map the byte value range to backlight command range
  byte red   = 128 + map(r, 0, 255, 0, 29);
  byte green = 158 + map(g, 0, 255, 0, 29);
  byte blue  = 188 + map(b, 0, 255, 0, 29);


  //send commands to the display to set backlights
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device

  //Turn display off to hide confirmation messages
  _displayControl &= ~LCD_DISPLAYON;
  _i2cPort->write(SPECIAL_COMMAND); //Send special command character
  _i2cPort->write(LCD_DISPLAYCONTROL | _displayControl);

  //Set the red, green and blue values
  _i2cPort->write(SETTING_COMMAND); //Set red backlight amount
  _i2cPort->write(red);
  _i2cPort->write(SETTING_COMMAND); //Set green backlight amount
  _i2cPort->write(green);
  _i2cPort->write(SETTING_COMMAND); //Set blue backlight amount
  _i2cPort->write(blue);

  //Turn display back on and end
  _displayControl |= LCD_DISPLAYON;
  _i2cPort->write(SPECIAL_COMMAND); //Send special command character
  _i2cPort->write(LCD_DISPLAYCONTROL | _displayControl); //Turn display on as before
  _i2cPort->endTransmission(); //Stop I2C transmission
  delay(50); //This one is a bit slow
} // setBacklight
/* New backlight function
void QwiicSerLCD::setBacklight(byte r, byte g, byte b) {

  //send commands to the display to set backlights
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device
  _i2cPort->write(SETTING_COMMAND); //Send special command character
  _i2cPort->write(SET_RGB_COMMAND); //Send the set RGB character '+' or plus
  _i2cPort->write(r); //Send the red value
  _i2cPort->write(g); //Send the green value
  _i2cPort->write(b); //Send the blue value
  _i2cPort->endTransmission(); //Stop I2C transmission
  delay(10);
 } // setBacklight
*/
/*
 * Set the text to flow from left to right.  This is the direction
 * that is common to most Western languages.
 */
void QwiicSerLCD::leftToRight() {
  _displayMode |= LCD_ENTRYLEFT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} // leftToRight

/*
 * Set the text to flow from right to left.
 */
void QwiicSerLCD::rightToLeft() {
  _displayMode &= ~LCD_ENTRYLEFT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} //rightToLeft

/*
 * Turn autoscrolling on. This will 'right justify' text from
 * the cursor.
 */
void QwiicSerLCD::autoscroll() {
  _displayMode |= LCD_ENTRYSHIFTINCREMENT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} //autoscroll
/*
 * Turn autoscrolling off.
 */
void QwiicSerLCD::noAutoscroll() {
  _displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
  specialCommand(LCD_ENTRYMODESET | _displayMode);
} //noAutoscroll

/*
 * Change the contrast from 0 to 255. 120 is default.
 *
 * byte new_val - new contrast value
 */
void QwiicSerLCD::setContrast(byte new_val) {
  //send commands to the display to set backlights
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device
  _i2cPort->write(SETTING_COMMAND); //Send contrast command
  _i2cPort->write(CONTRAST_COMMAND); //0x18
  _i2cPort->write(new_val); //Send new contrast value
  _i2cPort->endTransmission(); //Stop I2C transmission

  delay(10); //Wait a little bit
} //setContrast

/*
 * Change the I2C Address. 0x72 is the default.
 * Note that this change is persistent.  If anything
 * goes wrong you may need to do a hardware reset
 * to unbrick the display.
 *
 * byte new_addr - new i2c address
 */
void QwiicSerLCD::setAddress(byte new_addr) {
  //send commands to the display to set backlights
  _i2cPort->beginTransmission(_i2cAddr); // transmit to device on old address
  _i2cPort->write(SETTING_COMMAND); //Send contrast command
  _i2cPort->write(ADDRESS_COMMAND);  //0x19
  _i2cPort->write(new_addr); //Send new contrast value
  _i2cPort->endTransmission(); //Stop I2C transmission

  //Update our own address so we can still talk to the display
  _i2cAddr = new_addr;


  delay(50); //This may take awhile
} //setContrast