#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>
#include <unistd.h>
#include <string.h>
#include <sys/alt_alarm.h>
#include <sys/alt_timestamp.h>
#include "pitches.h"

#define WRITEDATA(base, data) IOWR_ALTERA_AVALON_PIO_DATA(base, data)
#define READDATA(base) IORD_ALTERA_AVALON_PIO_DATA(base)
