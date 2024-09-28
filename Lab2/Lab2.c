#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_MESSAGE_SIZE 1048576  // 1 MB
#define NUM_TRIALS 10

double measure_time(int message_size) {
    MPI_Status status;
    char *message = (char *) malloc(message_size * sizeof(char));
    double start, end;

    // Синхронизация процессов
    MPI_Barrier(MPI_COMM_WORLD);

    // Время начала
    start = MPI_Wtime();
    if (MPI_Send(message, message_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        perror("MPI_Send failed");
    }

    if (MPI_Recv(message, message_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status) != MPI_SUCCESS) {
        perror("MPI_Recv failed");
    }
    // Время конца
    end = MPI_Wtime();

    free(message);
    return end - start;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int message_size = 1; message_size <= MAX_MESSAGE_SIZE; message_size *= 2) {
        if (rank == 0) {
            double total_time = 0.0;
            for (int i = 0; i < NUM_TRIALS; i++) {
                total_time += measure_time(message_size);
            }
            double average_time = total_time / NUM_TRIALS;
            printf("Message size: %d bytes, Time: %f seconds\n", message_size, average_time);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}