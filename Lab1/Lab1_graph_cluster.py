import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict

processes_to_times = defaultdict(list)

try:
    with open('results_cluster.txt', 'r') as f:
        for line in f:
            parts = line.split(' ')
            processes = int(parts[1].split(',')[0])
            time = float(parts[3])
            processes_to_times[processes].append(time)

except FileNotFoundError:
    print("Ошибка")
    exit()

except ValueError:
    print("Ошибка")
    exit()

serial_time = processes_to_times[1][0]
average_speedup = {}
std_errors = {}
for processes, times in processes_to_times.items():
    avg_time = np.mean(times)  # Среднее время для данного числа процессов
    std_error = np.std(times) / np.sqrt(len(times))  # Стандартная ошибка
    average_speedup[processes] = serial_time / avg_time  # Вычисление ускорения
    std_errors[processes] = std_error

processes = list(average_speedup.keys())
speedup = list(average_speedup.values())
errors = [serial_time / (avg_time ** 2) * std_error for avg_time, std_error in zip([np.mean(processes_to_times[p]) for p in processes], [std_errors[p] for p in processes])]

degree = 7
coefficients = np.polyfit(processes, speedup, degree)
polynomial = np.poly1d(coefficients)

processes_fit = np.linspace(min(processes), max(processes), 1000)
speedup_fit = polynomial(processes_fit)

plt.figure(figsize=(10, 6))
plt.errorbar(processes, speedup, yerr=errors, label='Среднее ускорение', color='blue', marker='o', capsize=5, linestyle='None')
plt.plot(processes_fit, speedup_fit, label='Аппроксимирующая кривая', color='red', linestyle='--')
plt.title('Среднее ускорение параллельного алгоритма на кластере \n (множественные запуски) \n для N = 10^6', fontsize=16, fontweight='bold')
plt.xlabel('Число процессов', fontsize=14)
plt.ylabel('Среднее ускорение', fontsize=14)
plt.xticks(processes, fontsize=12)
plt.yticks(fontsize=12)
plt.grid(color='gray', linestyle='--', linewidth=0.5, alpha=0.7)
plt.legend()
plt.tight_layout()
plt.savefig('average_speedup_graph_cluster.png', dpi=300)
plt.show()