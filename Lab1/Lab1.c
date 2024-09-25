#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double f(double x) {
    return sqrt(4 - x * x);
}

double trapezoidal_rule(double a, double b, int N) {
    double h = (b - a) / N;
    double sum = (f(a) + f(b)) / 2.0;

    for (int i = 1; i < N; i++) {
        sum += f(a + i * h);
    }
    return sum * h;
}

int main(int argc, char *argv[]) {
    int rank, size;
    double a = 0.0, b = 2.0;
    int N, rank_N;
    double rank_a, rank_b, rank_integral, total_integral;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        N = atoi(argv[1]);
    }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Определяем количество отрезков и границы для каждого процесса
    rank_N = N / size;
    rank_a = a + rank * rank_N * (b - a) / N;
    if (rank == size - 1) {
        rank_b = b;
    } else {
        rank_b = a + (rank + 1) * rank_N * (b - a) / N;
    }

    // Последний процесс берёт дополнительные разбиения, в случае если есть остаток
    if (rank == size - 1) {
        rank_N += N % size;
    }

    start_time = MPI_Wtime();
    rank_integral = trapezoidal_rule(rank_a, rank_b, rank_N);
    MPI_Reduce(&rank_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    printf("Rank %d: Интеграл = %f\n", rank, rank_integral);
    if (rank == 0){
        printf("Значение интеграла: %.6f\n", total_integral);
        printf("Астрономическое время: %f секунд\n", end_time - start_time);

        FILE *f = fopen("results.txt", "a");
        fprintf(f, "Processes: %d, Time: %f seconds, Integral: %f\n", size, end_time - start_time, total_integral);
        fclose(f);
    }

    MPI_Finalize();

    return 0;
}