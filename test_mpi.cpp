//
// Created by Ammar on 12/23/16.
//
/**
 * mpi playground.
 */

#ifndef PORR_TESTMPI_H
#define PORR_TESTMPI_H

#include <iostream>
#include <mpi.h>
#include "porr.h"
#include "MPIHelper.h"

using namespace std;


void run_master(int rank, int size);
void run_slave(int rank, int size);



void run_master(int rank, int size) {
    cout << "M:starting " << rank << " as a master" << endl;
    const char *msg = "Hello World";
    MPI_Send(msg, (int) strlen(msg) + 1, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
    cout << "M:Sent msg to " << 1 << " " << endl;
}

int main(int argc, char* argv[]) {
    MPIHelper::init();
    int size = MPIHelper::size();
    int rank = MPIHelper::rank();

    if (rank == 0) {
        // master
        cout << "Master " << endl;
        cout << "Doin nothing. Just chilling " << endl;

    } else {
        // slave

        int m = 20;
        KnapsackProblem p = KnapsackProblemFactory::createSimpleProblem(1000, m);
        SeqBruteForceSolver bfs;

        int workersSize = size-1;
        int solutions = 1 << m;
        int perWorker = solutions/workersSize;

        int from = (rank-1)*perWorker;
        int to = (rank) * perWorker;

        if (rank == size - 1) {
            // last worker gets the most
            to += (solutions % workersSize);
        }


        cout << "Rank = " << rank << endl;
        cout << "solutions: " << solutions << endl;
        cout << "perWorker: " << perWorker << endl;
        cout << "R(" << rank << "): from: " << from << endl;
        cout << "R(" << rank << "): to: " << to << endl;

        long d = bfs.solve(p, from, to);

        cout << "R(" << rank << "): maxValue: " << d << endl;

    }
    MPIHelper::finalize();
    return 0;
}


int main2(int argc, char* argv[]) {
    int rank, size;
    const int MASTER_RANK = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    string data = argv[1];

    cout << data << endl;
    if (rank == MASTER_RANK) {
        run_master(rank, size);
    } else {
        run_slave(rank, size);
    }
    return 0;
}

void run_slave(int rank, int size) {
    cout << "S:starting " << rank << " as a slave" << endl;
    char buffer[100];
    MPI_Recv(buffer, 100, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    cout << "S:Received a message from master" << endl;
    cout << "S: msg = " << buffer << endl;
}




#endif //PORR_TESTMPI_H
