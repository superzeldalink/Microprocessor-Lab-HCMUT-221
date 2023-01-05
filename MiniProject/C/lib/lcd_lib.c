#include "lcd_lib.h"

void lcd_write(int data)
{
	WRITEDATA(LCD_BASE, data | 0b00100000000);
	WRITEDATA(LCD_BASE, data & 0b11011111111);
	WRITEDATA(LCD_BASE, data | 0b00100000000);
	usleep(50);
}
void lcd_init()
{
	usleep(15000);
	lcd_write(0b00100111000);
	usleep(5000);
	lcd_write(0b00100001100);	// Display On/Off control
	usleep(50);
	lcd_write(0b00100000110);   // Entry mode set
	usleep(100);
	lcd_write(0b00100000001);   // Clear screen
	usleep(2000);
}

void lcd_printtext(char *string) {
	for (int i = 0; i < strlen(string); i++)
		lcd_senddata(string[i]);
}

void lcd_senddata(char data) {
	lcd_write(0b10100000000 + data);
}

void lcd_sendcmd(char cmd) {
	lcd_write(0b00100000000 + cmd);
}

void lcd_setcursor(char row, char col) {
	int row_char = 0;
	if (row == 1) row_char = 64;
	else if (row == 2) row_char = 20;
	else if (row == 3) row_char = 84;
	lcd_sendcmd(0b00010000000 + row_char + col);
}
