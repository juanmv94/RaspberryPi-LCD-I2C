#ifndef FDB_LIQUID_CRYSTAL_I2C_H
#define FDB_LIQUID_CRYSTAL_I2C_H

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

/**
* Set the LCD display in the correct begin state, must be called before anything else is done.
*
* @param lcd_addr	I2C slave address of the LCD display. Most likely printed on the
*					LCD circuit board, or look in the supplied LCD documentation.
* @param lcd_cols	Number of columns your LCD display has.
* @param lcd_rows	Number of rows your LCD display has.
* @param charsize	The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
*/
void LCD_begin(char lcd_addr, char lcd_cols, char lcd_rows, char charsize);

/**
* Remove all the characters currently shown. Next print/write operation will start
* from the first position on LCD display.
*/
void LCD_clear();

/**
* Next print/write operation will will start from the first position on the LCD display.
*/
void LCD_home();

/**
* Do not show any characters on the LCD display. Backlight state will remain unchanged.
* Also all characters written on the display will return, when the display in enabled again.
*/
void LCD_noDisplay();

/**
* Show the characters on the LCD display, this is the normal behaviour. This method should
* only be used after LCD_noDisplay() has been used.
*/
void LCD_display();

/**
* Do not blink the cursor indicator.
*/
void LCD_noBlink();

/**
* Start blinking the cursor indicator.
*/
void LCD_blink();

/**
* Do not show a cursor indicator.
*/
void LCD_noCursor();

/**
* Show a cursor indicator, cursor can blink on not blink. Use the
* methods LCD_blink() and LCD_noBlink() for changing cursor blink.
*/
void LCD_cursor();

void LCD_scrollDisplayLeft();
void LCD_scrollDisplayRight();
void LCD_printLeft();
void LCD_printRight();
void LCD_leftToRight();
void LCD_rightToLeft();
void LCD_shiftIncrement();
void LCD_shiftDecrement();
void LCD_noBacklight();
void LCD_backlight();
int LCD_getBacklight();
void LCD_autoscroll();
void LCD_noAutoscroll();
void LCD_createChar(char, char[]);
void LCD_setCursor(char, char);
void LCD_print(char*);
void LCD_write(char);
void LCD_command(char);

#endif // FDB_LIQUID_CRYSTAL_I2C_H
