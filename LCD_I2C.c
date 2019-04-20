#include "LCD_I2C.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

char _addr;
char _displayfunction;
char _displaycontrol;
char _displaymode;
char _cols;
char _rows;
char _charsize;
char _backlightval;

int fd;

/************ low level data pushing LCD_commands **********/

static void expanderWrite(char _data){
	wiringPiI2CWrite(fd,(int)(_data) | _backlightval);
}

static void pulseEnable(char _data){
	expanderWrite(_data | En);	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns

	expanderWrite(_data & ~En);	// En low
	delayMicroseconds(50);		// LCD_commands need > 37us to settle
}

static void write4bits(char value) {
	expanderWrite(value);
	pulseEnable(value);
}

// write either LCD_command or data
static void send(char value, char mode) {
	char highnib=value&0xf0;
	char lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

void LCD_begin(char lcd_addr, char lcd_cols, char lcd_rows, char charsize) {
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;
	fd=wiringPiI2CSetup((int)_addr);
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1) {
		_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending LCD_commands. Arduino can turn on way befer 4.5V so we'll wait 50
	delay(50);

	// Now we pull both RS and R/W low to begin LCD_commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	delay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	delayMicroseconds(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	LCD_command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCD_display();

	// clear it off
	LCD_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	LCD_command(LCD_ENTRYMODESET | _displaymode);

	LCD_home();
}

/********** high level LCD_commands, for the user! */
void LCD_clear(){
	LCD_command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	delayMicroseconds(2000);  // this LCD_command takes a long time!
}

void LCD_home(){
	LCD_command(LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this LCD_command takes a long time!
}

void LCD_setCursor(char col, char row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	LCD_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LCD_noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD_display() {
	_displaycontrol |= LCD_DISPLAYON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LCD_noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD_cursor() {
	_displaycontrol |= LCD_CURSORON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LCD_noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD_blink() {
	_displaycontrol |= LCD_BLINKON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These LCD_commands scroll the display without changing the RAM
void LCD_scrollDisplayLeft(void) {
	LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_scrollDisplayRight(void) {
	LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCD_leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LCD_rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LCD_autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LCD_noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD_createChar(char location, char charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	LCD_command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		LCD_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void LCD_noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LCD_backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}
int LCD_getBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}

void LCD_print(char* string) {
	char* c=string;
	while (*c != 0)
		LCD_write(*(c++));
}


/*********** mid level LCD_commands, for sending data/cmds */

void LCD_command(char value) {
	send(value, 0);
}

void LCD_write(char value) {
	send(value, Rs);
}
