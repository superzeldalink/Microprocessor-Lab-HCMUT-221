import csv

output = ""

with open('mat.csv') as csv_file:
    reader = csv.reader(csv_file, delimiter=',')
    rows = list(reader)
    
    name_a, row_a, col_a = rows[0][0], int(rows[0][1]), int(rows[0][3])
    name_b, row_b, col_b = rows[10][0], int(rows[10][1]), int(rows[10][3])
    
    if col_a != row_b:
        print("ERROR!")
        exit()
    
    output += f'\t{name_a} = [\n'
    for i in range(0, row_a):
        row = rows[i+1]
        output += f'\t\t'
        for j in range(0, col_a):
            output += f'{row[j]},'
        output = f'{output[:-1]};\n'
    output += "\t]\n"

    output += f'\t{name_b} = [\n'
    for i in range(0, row_b):
        row = rows[i+11]
        output += f'\t\t'
        for j in range(0, col_b):
            output += f'{row[j]},'
        output = f'{output[:-1]};\n'
    output += "\t]\n"
    
file = open("mat.m", "w")
file.write(output)
file.close()
