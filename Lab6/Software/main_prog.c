#include "mm_lib.h"
#include <sys/alt_timestamp.h>

double timestamp() {
  alt_u32 timestamp = alt_timestamp();
  return timestamp * 1.0 / alt_timestamp_freq();
}

int main2() {
	printf("Ready!\n");
	int a[8][8] = {
		{8,0,0,9,15,9,9,6},
		{0,9,6,7,1,6,0,6},
		{0,14,6,9,6,1,8,15},
		{8,15,14,7,8,8,9,1},
		{15,7,7,0,14,0,14,14},
		{8,7,14,6,15,14,8,1},
		{1,14,8,14,6,1,0,15},
		{6,1,7,14,7,1,8,7}
	};

	int b[8][8] = {
		{7,14,0,6,7,8,1,15},
		{15,6,15,0,6,6,14,9},
		{14,6,8,6,8,15,8,9},
		{15,0,6,1,8,1,0,0},
		{9,7,8,9,15,1,6,0},
		{8,7,0,6,6,6,1,8},
		{6,1,8,14,1,9,14,15},
		{8,15,9,9,1,0,9,7}
	};

	int ab[8][8];

	mm_set_address(0);
	mm_send_matrix(a, 8, 8);
	mm_send_matrix(b, 8, 8);
	mm_read_matrix(ab, 8, 8);

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			printf("%d, ", *(*(ab + i)+j));
		}
		printf("\n");
	}

  return 0;
}
