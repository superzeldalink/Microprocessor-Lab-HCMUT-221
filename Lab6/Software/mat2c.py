import csv

output = """#include "mm_lib.h"

int main() {
	printf("Ready!\\n");
"""

with open('mat.csv') as csv_file:
    reader = csv.reader(csv_file, delimiter=',')
    rows = list(reader)
    
    name_a, row_a, col_a = rows[0][0], int(rows[0][1]), int(rows[0][3])
    name_b, row_b, col_b = rows[10][0], int(rows[10][1]), int(rows[10][3])
    
    if col_a != row_b:
        print("ERROR!")
        exit()
    
    output += f'\tint {name_a}[{row_a}][{col_a}] = {{\n'
    for i in range(0, row_a):
        row = rows[i+1]
        output += f'\t\t{{'
        for j in range(0, col_a):
            output += f'{row[j]},'
        output = f'{output[:-1]}}},\n'
    output += "\t};\n"

    output += f'\tint {name_b}[{row_b}][{col_b}] = {{\n'
    for i in range(0, row_b):
        row = rows[i+11]
        output += f'\t\t{{'
        for j in range(0, col_b):
            output += f'{row[j]},'
        output = f'{output[:-1]}}},\n'
    output += "\t};\n"
    
    output += f'\tint ab[{row_a}][{col_b}];\n'
    output += f"""
	mm_set_address(0);
	mm_send_matrix({name_a}, {row_a}, {col_a});
	mm_send_matrix({name_b}, {row_b}, {col_b});
	mm_read_matrix(ab, {row_a}, {col_b});

	for(int i = 0; i < {row_a}; i++){{
		for(int j = 0; j < {col_b}; j++)
			printf("%d, ", *(*(ab + i)+j));
		printf("\\n");
	}}
}}"""
    
file = open("prog.c", "w")
file.write(output)
file.close()
