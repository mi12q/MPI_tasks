import matplotlib.pyplot as plt

processes = []
times = []

with open('results_cluster.txt', 'r') as f:
    for line in f:
        parts = line.split(' ')
        processes.append(int(parts[1].split(',')[0]))
        times.append(float(parts[3]))

# Расчет ускорения
serial_time = times[0]
speedup = [serial_time / t for t in times]

plt.figure(figsize=(10, 6))
plt.scatter(processes, speedup)
plt.title('Ускорение параллельного алгоритма на кластере \n (программа запущена 10 раз) \n при N = 10^6', fontsize=16, fontweight='bold')
plt.xlabel('Число процессов', fontsize=14)
plt.ylabel('Ускорение', fontsize=14)
plt.xticks(processes, fontsize=12)
plt.yticks(fontsize=12)
plt.grid(color='gray', linestyle='--', linewidth=0.5, alpha=0.7)

plt.tight_layout()
plt.savefig('speedup_graph_cluster_.png', dpi=300)
plt.show()
