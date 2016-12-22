//
// Created by Ammar on 12/23/16.
//

#ifndef PORR_TESTMPI_H
#define PORR_TESTMPI_H

#include <iostream>
#include <gtest/gtest.h>
#include <mpi.h>

/**
 * test mpi features
 */
using namespace std;

TEST(TestMPI, HelloWorld) {
    int rank, size;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char proessorName[MPI_MAX_PROCESSOR_NAME];
    int nameLen;
    MPI_Get_processor_name(proessorName, &nameLen);


    cout << "Hello world from "<< proessorName << " rank " << rank << " of " << size << endl;


    MPI_Finalize();

}

void run_master(int rank, int size);
void run_slave(int rank, int size);



int main(int argc, char* argv[]) {
    int rank, size;
    const int MASTER_RANK = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER_RANK) {
        run_master(rank, size);
    } else {
        run_slave(rank, size);
    }
 }

void run_slave(int rank, int size) {
    cout << "S:starting " << rank << " as a slave" << endl;
    char buffer[100];
    MPI_Recv(buffer, 100, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    cout << "S:Received a message from master" << endl;
    cout << "S: msg = " << buffer << endl;
}

void run_master(int rank, int size) {
    cout << "M:starting " << rank << " as a master" << endl;
    const char *msg = "Hello World";
    MPI_Send(msg, (int) strlen(msg) + 1, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
    cout << "M:Sent msg to " << 1 << " " << endl;
}

int main1(int argc, char*argv[]) {
   ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif //PORR_TESTMPI_H
