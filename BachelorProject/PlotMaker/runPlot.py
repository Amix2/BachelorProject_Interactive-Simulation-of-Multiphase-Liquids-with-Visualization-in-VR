from matplotlib import pyplot
import sys
import math
from scipy import optimize
import numpy as np

def fun_n_log_n(xArr, a, b, d):
    if( xArr.__class__.__name__ == 'ndarray'):
        return [ a * x * math.log(b * x) + d  for x in xArr ]
    else:
        x = xArr
        return a * x * math.log(b * x) ** 2 + d


def fun_n_log2_n(xArr, a, b, d):

    if( xArr.__class__.__name__ == 'ndarray'):
        return [ a * x * math.log(b * x) ** 2 + d  for x in xArr]
    else:
        x = xArr
        return a * x * math.log(b * x) ** 2 + d

def fun_2_n_log2_n(xArr, a, b, d):
    
    if( xArr.__class__.__name__ == 'ndarray'):
        
        return [ a * (2**(math.floor(math.log2(x))+1)) * (math.log(b * (2**(math.floor(math.log2(x))+1))) ** 2) + d  for x in xArr]
    else:
        x = xArr
        return a * (2**(math.floor(math.log2(x))+1)) * (math.log(b * (2**(math.floor(math.log2(x))+1))) ** 2) + d

def fun_n(x, a, b):
        return a*x + b

def fun_n2(x, a, b, c):
        return a*x*x + b*x + c

def test_fun(fun, name, xArr, yArr, color, **kwargs):
    try:
        params, params_covariance = optimize.curve_fit(fun, xArr, yArr, **kwargs)
    except RuntimeError:
        print(name, "failed")
    else:
        chisq = 0
        xs = []
        ys = []
        for x, y in zip(xArr, yArr):
            chisq += (y - fun(x, *[arg for arg in params]))**2
            xs.append(x)
            ys.append( fun(x, *[arg for arg in params]))
        chisq /= len(xArr) - 1
        print(name, params, "chisq", chisq)
        if(chisq < 10):
            pyplot.plot(xs, ys, color = color, label=name)

def test_plots(xArr, yArr):
    test_fun(fun_n, "n", xArr, yArr, 'g')
    test_fun(fun_n2, "n^2", xArr, yArr, 'm')

    test_fun(fun_n_log_n, "n log n", xArr, yArr, 'r', bounds=( [-np.inf, 0, -np.inf], [np.inf, np.inf, np.inf] ))

    test_fun(fun_n_log2_n, "n log^2 n", xArr, yArr, 'y', bounds=( [-np.inf, 0, -np.inf], [np.inf, np.inf, np.inf] ))

    test_fun(fun_2_n_log2_n, "n log^2 n, 2**", xArr, yArr, 'y', bounds=( [-np.inf, 0, -np.inf], [np.inf, np.inf, np.inf] ))

if(len(sys.argv) > 1):
    file_name = str(sys.argv[1])
else:
    file_name = "log.log"

file = open(file_name,"r")
file_2 = open('log_base_case.log',"r")
string = file.read()
string_2 = file_2.read()
sort_values = []
sph_values = []
sph2_values = []
lin_values = []
copy_values = []
part_count = []
part2_count = []


sort_string = string[ string.index('SORT ==================== SORT') + len('SORT ==================== SORT')+1 : string.index('SPH ==================== SPH') ]
for part_str in sort_string.split('),'):
    if(part_str.startswith(' ( ')):
        part_str = part_str[3:]
        values = part_str.split(', ')
        sort_values.append(float(values[1]))
        part_count.append(int(values[0]))
print("sort")
pyplot.title("Etap 3 symulacji")
pyplot.plot(part_count, sort_values, color = 'b', linewidth=0.2, label="Dane")
test_plots(part_count, sort_values)
pyplot.xlabel("Liczba cząstek")
pyplot.ylabel("Czas [ms]")
pyplot.legend()
pyplot.show()
pyplot.close()
part_count = []

# sph_string = string_2[ string.index('SPH ==================== SPH') + len('SPH ==================== SPH')+3 : string_2.index('LIN ==================== LIN')]
# for part_str in sph_string.split('),'):
#     if(part_str.startswith(' ( ')):
#         part_str = part_str[3:]
#         values = part_str.split(', ')
#         sph2_values.append(float(values[1]))
#         part2_count.append(int(values[0]))

# sph_string = string[ string.index('SPH ==================== SPH') + len('SPH ==================== SPH')+3 : string.index('LIN ==================== LIN')]
# for part_str in sph_string.split('),'):
#     if(part_str.startswith(' ( ')):
#         part_str = part_str[3:]
#         values = part_str.split(', ')
#         sph_values.append(float(values[1]))
#         part_count.append(int(values[0]))

# print("sph")
# pyplot.title("Etap 6 i Etap 7 symulacji")
# pyplot.plot(part_count, sph_values, color = 'b', linewidth=1, label="Zmieniony test")
# #pyplot.plot(part2_count, sph2_values, color = 'red', linewidth=1, label="Bazowy test")
# test_plots(part_count, sph_values)
# pyplot.xlabel("Liczba cząstek")
# pyplot.ylabel("Czas [ms]")
# pyplot.legend()
# pyplot.show()
# pyplot.close()
# part_count = []


# lin_string = string[ string.index('LIN ==================== LIN') + len('LIN ==================== LIN')+1 : string.index('COPY ==================== COPY')]
# for part_str in lin_string.split('),'):
#     if(part_str.startswith(' ( ')):
#         part_str = part_str[3:]
#         values = part_str.split(', ')
#         lin_values.append(float(values[1]))
#         part_count.append(int(values[0]))
# print("lin")
# pyplot.title("Etap 2, Etap 4 i Etap 8 symulacji")
# pyplot.plot(part_count, lin_values, color = 'b', linewidth=0.2, label="Dane")
# test_plots(part_count, lin_values)
# pyplot.xlabel("Liczba cząstek")
# pyplot.ylabel("Czas [ms]")
# pyplot.legend()
# pyplot.show()
# pyplot.close()
# part_count = []

# copy_string = string[ string.index('COPY ==================== COPY') + len('COPY ==================== COPY')+2 :]
# for part_str in copy_string.split('),'):
#     if(part_str.startswith(' ( ')):
#         part_str = part_str[3:]
#         values = part_str.split(', ')
#         copy_values.append(float(values[1]))
#         part_count.append(int(values[0]))
# print("copy")
# pyplot.title("Kopiowanie kontenerów z Etapu 2 symulacji")
# pyplot.plot(part_count, copy_values, color = 'b', linewidth=0.2, label="Dane")
# test_plots(part_count, copy_values)
# pyplot.xlabel("Liczba cząstek")
# pyplot.ylabel("Czas [ms]")
# pyplot.legend()
# pyplot.show()
# pyplot.close()
# part_count = []