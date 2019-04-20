#include "LCD_I2C.h"

void main() {
 LCD_begin(0x27, 20, 4, LCD_5x8DOTS);
 LCD_print("Raspberry Pi");
}
