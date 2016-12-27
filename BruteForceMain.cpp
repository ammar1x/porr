#include <iostream>

#include "Configuration.h"
#include "KnapsackProblem.h"
#include "SeqBruteForce.h"
#include "OCLBruteForce.h"
#include "OCLHelper.h"
#include <mpi.h>
#include "MPIHelper.h"


using namespace std;





int runMPIBruteForce(string filePath) {

    MPIHelper::init();

    int size = MPIHelper::size();
    int rank = MPIHelper::rank();
    const int MASTERS_RANK = 0;
    const int MAX_LEN = 10000;

    if (rank == MASTERS_RANK) {
        std::string path = filePath;

        cout << "M: Reading configuration from file = " << path << endl;
        Configuration c = Configuration::fromFile(path);
        std::string cjson = c.toJson().dump(4);
        cout << "M: broadcasting configuration" << endl;
        MPIHelper::broadcast(cjson, MASTERS_RANK);

        vector<int64_t> res(size, -1);
        int64_t fake = -1;
        MPI_Gather(&fake, 1, MPI_INT64_T, res.data(), 1, MPI_INT64_T, MASTERS_RANK, MPI_COMM_WORLD);
        long maxVal = 0;
        for_each(res.begin(), res.end(), [&maxVal](long a) {
            maxVal = std::max(a, maxVal);
        });
        cout << "M: optimal value = " << maxVal << endl;

    } else {
        cout << "S(" << rank << ")" << " waiting for configuration" <<endl;
        std::string cjson = MPIHelper::broadcastRecv<std::string>(MASTERS_RANK);
        cout << "S(" << rank << ")" << " Received configuration" <<endl;
        Configuration c = Configuration::fromJson(cjson);

        KnapsackProblem p = getKnapsackProblems(c)[0]; // get the first one

        int workersSize = size-1;
        int solutions = 1 << (p.objectsValues.size());
        int perWorker = solutions/workersSize;

        int from = (rank-1)*perWorker;
        int to = (rank) * perWorker;

        if (rank == size - 1) {
            // last worker gets the most
            to += (solutions % workersSize);
        }

        SeqBruteForceSolver bfs;
        int64_t d = bfs.solve(p, from, to);

        cout << "Rank = " << rank << endl;
        cout << "solutions: " << solutions << endl;
        cout << "perWorker: " << perWorker << endl;
        cout << "R(" << rank << "): from: " << from << endl;
        cout << "R(" << rank << "): to: " << to << endl;
        cout << "R(" << rank << "): solution: " << d << endl;

        MPI_Gather(&d, 1, MPI_INT64_T, NULL, 0, MPI_INT64_T, MASTERS_RANK, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPIHelper::finalize();
}



int runOclBruteForce(string filePath) {
    Configuration conf = Configuration::fromFile(filePath);
    cout << conf.toBasicJson().dump(2) << endl;

    vector<KnapsackProblem> problems = getKnapsackProblems(conf);

    vector<cl::Device> devices = OCLHelper::getDevices(CL_DEVICE_TYPE_GPU);
    if (devices.empty()) {
        devices = OCLHelper::getDevices();
    }

    auto device = devices[0];
    cout << OCLHelper::getDeviceInfo(device) << endl;

    cl::Context context({device});

    OCLBruteForce solver;

    solver.setDevice(&device);
    solver.setContext(&context);
    solver.buildProgram();


    for(KnapsackProblem& problem : problems) {
        cout << solver.solve(problem) << endl;
    }
    return 0;
}

int runSeqBruteForce(string filePath) {


    Configuration conf = Configuration::fromFile(filePath);
    cout << conf.toBasicJson().dump(2) << endl;

    vector<KnapsackProblem> problems = getKnapsackProblems(conf);

    SeqBruteForceSolver solver;

    for(KnapsackProblem& problem : problems) {
        cout << solver.solve(problem) << endl;
    }

    return 0;
}

void showUsage(int argc, char*argv[]) {
    cout << "usage: " << endl;
    cout << argv[0] << " <<mode>>  <<filePath>>" << endl;
    cout << "mode = {opencl, seq, mpi}" << endl;
    cout << "filePath = path to problem file" << endl;
}
int main(int argc, char* argv[]) {

    if (argc < 3) {
        showUsage(argc, argv);
        exit(1);
    }

    string mode = argv[1];
    string filePath = argv[2];

    cout << "using " << mode << " to solve optimization problem" << endl;

    if (mode == "opencl") {
        runOclBruteForce(filePath);
    } else if (mode == "seq"){
        runSeqBruteForce(filePath);
    } else if (mode == "mpi") {
        runMPIBruteForce(filePath);
    }
    return 0;
}