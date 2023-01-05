#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>
#include <string.h>

#define WRITEDATA(base, data) IOWR_ALTERA_AVALON_PIO_DATA(base, data)
#define READDATA(base) IORD_ALTERA_AVALON_PIO_DATA(base)

void lcd_write(int data);
void lcd_init();
void lcd_printtext(char *string);
void lcd_senddata(char data);
void lcd_sendcmd(char cmd);
void lcd_setcursor(char row, char col);
