# Raspberry Pi LCD I2C

## About this library

This library allows you to use an I2C HD44780 based LCD display in the C programming lenguaje with Raspberry Pi using [WiringPi](http://http://wiringpi.com/).

It's based on the C++ LCD I2C library from Arduino, and their commands are similar.

## Getting Started

To make this lib to work, you will need the following:

* Download and install [WiringPi](http://http://wiringpi.com/) (if you don't have it yet).
* Enable I2C in **raspi-config**.
  * Run **sudo raspi-config**.
  * Use the down arrow to select 5 **Interfacing Options**.
  * Arrow down to P5 **I2C**.
  * Select **yes** when it asks you to enable I2C.
  * Reboot.
* Connect the LCD screen to the I2C pins (SDA, SCL) and power of Raspberry Pi.
* Check the address of your LCD screen with **i2cdetect -y 0** or **i2cdetect -y 1** depending your RPI version (you must install i2cdetect first with **sudo apt-get install i2c-tools**).

Then you can try to compile and run the example.

## Basic commands
Here are some of the most important functions of the library:

* **void LCD\_begin(char lcd\_addr, char lcd\_cols, char lcd\_rows, char charsize)**: You need to call this function first to initialize the library with the I2C address, the number of columns of your display, the number of rows of your display, and the char dot size (*LCD\_5x10DOTS* or *LCD\_5x8DOTS*).
* **void LCD\_noBacklight()**: disables screen backlight.
* **void LCD\_backlight()**: enables screen backlight.
* **void LCD\_write(char)**: writes a single character to the screen, and increases the cursor position.
* **void LCD\_print(char\*)**: writes a string to the screen, and sets the cursor position to the end of the string
* **void LCD\_setCursor(char, char)**: sets the cursor position to the desired coordinates.
* **void LCD\_home()**: Next print/write operation will will start from the first position on the LCD display.
* **void LCD\_clear()**: Remove all the characters currently shown. Next print/write operation will start from the first position on LCD display.
* **void LCD\_noDisplay()**: Do not show any characters on the LCD display. Backlight state will remain unchanged. Also all characters written on the display will return, when the display in enabled again.
* **void LCD\_display()**: Show the characters on the LCD display, this is the normal behaviour. This method should only be used after *LCD\_noDisplay()* has been used.
* **void LCD\_noBlink()**: Do not blink the cursor indicator.
* **void LCD\_blink()**: Start blinking the cursor indicator.
* **void LCD\_noCursor()**: Do not show a cursor indicator.
* **void LCD\_cursor()**: Show a cursor indicator, cursor can blink on not blink. Use the methods *LCD\_blink()* and *LCD\_noBlink()* for changing cursor blink.
* **void LCD\_createChar(char, char[])**: Use this to upload custom characters. Up to eight characters of 5x8 pixels are supported (numbered 0 to 7). The appearance of each custom character is specified by an array of eight bytes, one for each row. The five least significant bits of each byte determine the pixels in that row.