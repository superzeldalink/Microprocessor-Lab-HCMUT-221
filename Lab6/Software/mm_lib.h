#include <stdio.h>
#include <system.h>
#include <io.h>
#include <alt_types.h>
#include <stdlib.h>

#define MM_BASE MATMUL_BASE
#define MATRIX_MAX 8
#define WriteData(base, data) IOWR(base, 0, data)
#define ReadData(base) IORD(base, 0)

// WRITE: rw = 0
void mm_write(char data);
void mm_set_address(char addr);
void mm_send_data(char data);
void mm_send_data_with_address(char data, char start_addr);
void mm_send_matrix(int** arr, char row, char col);
void mm_send_matrix_with_address(int** arr, char row, char col, char start_addr);

// READ: rw = 1
char mm_read();
int mm_read_16();
void mm_read_matrix(int** ab, char row, char col);
