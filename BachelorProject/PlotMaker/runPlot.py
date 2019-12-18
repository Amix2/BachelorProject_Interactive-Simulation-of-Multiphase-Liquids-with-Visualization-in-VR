from matplotlib import pyplot
import sys

if(len(sys.argv) > 1):
    file_name = str(sys.argv[1])
else:
    file_name = "log.log"

file = open(file_name,"r")
string = file.read()
sort_values = []
sph_values = []
lin_values = []
copy_values = []
part_count = []
sort_string = string[ string.index('SORT ==================== SORT') + len('SORT ==================== SORT')+1 : string.index('SPH ==================== SPH') ]
for part_str in sort_string.split('),'):
    if(part_str.startswith(' ( ')):
        part_str = part_str[3:]
        values = part_str.split(', ')
        sort_values.append(float(values[1]))
        part_count.append(int(values[0]))

pyplot.title("Etap 3 symulacji")
pyplot.plot(part_count, sort_values, color = 'b')
pyplot.xlabel("Liczba cząstek")
pyplot.ylabel("Czas [ms]")
pyplot.show()
pyplot.close()
part_count = []

sph_string = string[ string.index('SPH ==================== SPH') + len('SPH ==================== SPH')+3 : string.index('LIN ==================== LIN')]
for part_str in sph_string.split('),'):
    if(part_str.startswith(' ( ')):
        part_str = part_str[3:]
        values = part_str.split(', ')
        sph_values.append(float(values[1]))
        part_count.append(int(values[0]))

pyplot.title("Etap 6 i Etap 7 symulacji")
pyplot.plot(part_count, sph_values, color = 'b')
pyplot.xlabel("Liczba cząstek")
pyplot.ylabel("Czas [ms]")
pyplot.show()
pyplot.close()
part_count = []


lin_string = string[ string.index('LIN ==================== LIN') + len('LIN ==================== LIN')+1 : string.index('COPY ==================== COPY')]
for part_str in lin_string.split('),'):
    if(part_str.startswith(' ( ')):
        part_str = part_str[3:]
        values = part_str.split(', ')
        lin_values.append(float(values[1]))
        part_count.append(int(values[0]))

    
pyplot.title("Etap 2, Etap 4 i Etap 8 symulacji")
pyplot.plot(part_count, lin_values, color = 'b')
pyplot.xlabel("Liczba cząstek")
pyplot.ylabel("Czas [ms]")
pyplot.show()
pyplot.close()
part_count = []

copy_string = string[ string.index('COPY ==================== COPY') + len('COPY ==================== COPY')+2 :]
for part_str in copy_string.split('),'):
    if(part_str.startswith(' ( ')):
        part_str = part_str[3:]
        values = part_str.split(', ')
        copy_values.append(float(values[1]))
        part_count.append(int(values[0]))

pyplot.title("Kopiowanie kontenerów z Etapu 2 symulacji")
pyplot.plot(part_count, copy_values, color = 'b')
pyplot.xlabel("Liczba cząstek")
pyplot.ylabel("Czas [ms]")
pyplot.show()
pyplot.close()
part_count = []