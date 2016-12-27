//
// Created by Ammar on 12/27/16.
//

#ifndef PORR_MPIHELPER_H
#define PORR_MPIHELPER_H

#include <mpi.h>


/**
 * Helper functions for mpi api.
 */
struct MPIHelper {

    static void init() {
        MPI_Init(0, 0);
    }

    static int size() {
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        return size;
    }

    static int rank() {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        return rank;
    }

    static void broadcast(std::string s, int root) {
        int n = s.size() + 1;
        constexpr int MAX_LEN = 10000;
        static char buffer[MAX_LEN];
        memcpy(buffer, s.c_str(), n);
        MPI_Bcast(buffer, MAX_LEN, MPI_CHAR, root, MPI_COMM_WORLD);
    }

    template<class T>
    static T broadcastRecv(int root) {
        constexpr  int MAX_LEN = 10000;
        static char buffer[MAX_LEN];
        MPI_Bcast(buffer, MAX_LEN, MPI_CHAR, root, MPI_COMM_WORLD);
        return T(buffer);
    }


    static void broadcast(std::string s, int root, int size) {
        int n = s.size() + 1;

        for(int i = 0; i < size; ++i) {
            if(root != i) {
                MPI_Send(s.c_str(), n, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
    }



    static std::string broadcastRecvStr(int root) {
        const int MAX_LEN = 100000;
        static char buffer[MAX_LEN];
        MPI_Bcast(buffer, MAX_LEN, MPI_CHAR, root, MPI_COMM_WORLD);
        return std::string(buffer);
    }

    static void finalize() {
        MPI_Finalize();
    }

    static void send(std::string msg, int dst, int tag=0) {
        int n = msg.size() + 1;
        MPI_Send(msg.c_str(), n , MPI_CHAR, dst, tag, MPI_COMM_WORLD);
    }



    /**
     *
     */
    static std::string recvStr(int src, int tag=0) {
        const int MAX_LEN = 10000;
        static char buf[MAX_LEN]; /* max */
        MPI_Recv(buf, MAX_LEN, MPI_CHAR, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return std::string(buf);
    }

};

#endif //PORR_MPIHELPER_H
