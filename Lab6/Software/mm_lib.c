#include "mm_lib.h"

// WRITE: rw = 0
void mm_write(char data){
	WriteData(MM_BASE, data & 0xFF);
}
void mm_set_address(char addr) {
	mm_write(0b10000000 + (addr & 0x7F));
}
void mm_send_data(char data) {
	mm_write(data & 0x7F);
}
void mm_send_data_with_address(char data, char start_addr) {
	mm_set_address(start_addr);
	mm_write(data & 0x7F);
}
void mm_send_matrix(int** arr, char row, char col){
	for(int i = 0; i < MATRIX_MAX; i++)
		for(int j = 0; j < MATRIX_MAX; j++)
			if(j >= col || i >= row) mm_send_data(0);
			else mm_send_data(*((int*)arr + col*i + j));
}
void mm_send_matrix_with_address(int** arr, char row, char col, char start_addr){
	mm_set_address(start_addr);
	mm_send_matrix(arr, row, col);
}

// READ: rw = 1
char mm_read(){
	return ReadData(MM_BASE);
}
int mm_read_16(){
	char t = mm_read();
	char t2 = mm_read();
	return (t << 8) | (t2 & 0xff);
}
void mm_read_matrix(int** ab, char row, char col){
	mm_set_address(0);
	for(int i = 0; i < MATRIX_MAX; i++){
		for(int j = 0; j < MATRIX_MAX; j++){
			if(j >= col || i >= row) mm_read_16();
			else *((int*)ab + col*i + j) = mm_read_16();
		}
	}
}
