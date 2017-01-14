#include <iostream>

#include "Configuration.h"
#include "KnapsackProblem.h"
#include "SeqBruteForce.h"
#include "OCLBruteForce.h"
#include "OCLHelper.h"
#include <mpi.h>
#include "MPIHelper.h"
#include "Stopwatch.h"
#include "utils.h"


static LOGGING::LogStream& LOG = LOGGING::LogStream::global(LOGGING::info);

using LOGGING::endl;



int runMPIBruteForce(string filePath) {

    MPIHelper::init();

    int size = MPIHelper::size();
    int rank = MPIHelper::rank();
    const int MASTERS_RANK = 0;
    const int MAX_LEN = 10000;

    Stopwatch<> sw;
    sw.start();

    if (rank == MASTERS_RANK) {
        std::string path = filePath;

        cout << "M: Reading configuration from file = " << path << endl;

        Configuration c = Configuration::fromFile(path);
        std::string cjson = c.toJson().dump(4);
        LOG.DEBUG() <<  "M: broadcasting configuration" << endl;

        MPIHelper::broadcast(cjson, MASTERS_RANK);

        KnapsackProblem p = getKnapsackProblems(c)[0];
        int64_t workersSize = size - 1;
        int64_t one = 1;
        int64_t solutions = one << (p.objectsValues.size());
        int64_t perWorker = solutions/workersSize;

        LOG.INFO() << "number of schedulers = " << 1 << endl;
        LOG.INFO() << "number of workers = " << size - 1 << endl;
        LOG.INFO() << "total search space size = " << solutions << endl;
        LOG.INFO() << "search space per worker = " << perWorker << endl;

        vector<int64_t> res(size, -1);
        int64_t fake = -1;
        MPI_Gather(&fake, 1, MPI_INT64_T, res.data(), 1, MPI_INT64_T, MASTERS_RANK, MPI_COMM_WORLD);
        int64_t maxVal = 0;
        for_each(res.begin(), res.end(), [&maxVal](int64_t a) {
            maxVal = std::max(a, maxVal);
        });

        LOG.INFO() << "M: optimal value = " << maxVal << endl;
        LOG.INFO() << "time: " << sw.elapsed() / 1000  << " ms" << endl;

    } else {

        LOG.DEBUG() << "S(" << rank << ")" << " waiting for configuration" <<endl;
        std::string cjson = MPIHelper::broadcastRecv<std::string>(MASTERS_RANK);
        LOG.DEBUG() << "S(" << rank << ")" << " Received configuration" ;

        Configuration c = Configuration::fromJson(cjson);
        KnapsackProblem p = getKnapsackProblems(c)[0]; // get the first one

        int workersSize = size-1;
        int64_t one = 1;
        int64_t solutions = one << (p.objectsValues.size());
        int64_t perWorker = solutions/workersSize;

        int64_t from = (rank-1)*perWorker;
        int64_t to = (rank) * perWorker;

        if (rank == size - 1) {
            // last worker gets most of the work
            to += (solutions % workersSize);
        }

        SeqBruteForceSolver bfs;
        int64_t d = bfs.solve(p, from, to);

        LOG.DEBUG() << "Rank = " << rank << endl;
        LOG.DEBUG() << "solutions: " << solutions << endl;
        LOG.DEBUG() << "perWorker: " << perWorker << endl;
        LOG.DEBUG() << "R(" << rank << "): from: " << from << endl;
        LOG.DEBUG() << "R(" << rank << "): to: " << to << endl;
        LOG.DEBUG() << "R(" << rank << "): solution: " << d << endl;

        MPI_Gather(&d, 1, MPI_INT64_T, NULL, 0, MPI_INT64_T, MASTERS_RANK, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPIHelper::finalize();
    return 0;
}



int runOclBruteForce(string filePath) {

    LOG.DEBUG() << "running opencl version " << endl;
    LOG.DEBUG() << "file path " << filePath << endl;

    Stopwatch<> sw;
    Configuration conf = Configuration::fromFile(filePath);
    LOG.INFO() << conf.toBasicJson().dump(2) << endl;

    vector<KnapsackProblem> problems = getKnapsackProblems(conf);

    vector<cl::Device> devices = OCLHelper::getDevices(CL_DEVICE_TYPE_GPU);
    if (devices.empty()) {
        devices = OCLHelper::getDevices();
    }

    auto device = OCLHelper::chooseTheBest(devices);
    LOG.INFO() << OCLHelper::getDeviceInfo(device) << endl;


    cl::Context context({device});

    OCLBruteForce solver;

    sw.start();
    solver.setDevice(&device);
    solver.setContext(&context);
    solver.buildProgram();
    LOG.DEBUG() << "building took " << sw.elapsed() / 1000 << " ms" << endl;


    sw.start();

    int64_t sol = solver.solve(problems[0]);
    LOG.INFO() << "Optimal value = " << sol << endl;
    LOG.INFO() << "time: " << sw.elapsed() / 1000 << " ms" << endl;

    return 0;
}

int runSeqBruteForce(string filePath) {

    LOG.DEBUG() << "running sequential version " << endl;
    LOG.DEBUG() << "file path " << filePath << endl;

    Configuration conf = Configuration::fromFile(filePath);
    LOG.INFO() << conf.toBasicJson().dump(2) << endl;
    vector<KnapsackProblem> problems = getKnapsackProblems(conf);
    SeqBruteForceSolver solver;

    Stopwatch<> sw;
    sw.start();
    KnapsackProblem kp = problems[0];
//    int64_t sol = knapsack(kp.capacity, kp.objectsWeights.data(), kp.objectsValues.data(), kp.objectsValues.size());
    int64_t sol = solver.solve(problems[0]);
    LOG.INFO() << "Optimal value = " << sol << endl;
    LOG.INFO() << "time: " << sw.elapsed() / 1000 << " ms" << endl;

    return 0;
}

void showUsage(int argc, char*argv[]) {
    LOG.INFO() << "usage: " << endl;
    LOG.INFO() << argv[0] << " <<mode>>  <<filePath>>" << endl;
    LOG.INFO() << "mode = {opencl, seq, mpi}" << endl;
    LOG.INFO() << "filePath = path to problem file" << endl;
}
int main(int argc, char* argv[]) {

    if (argc < 3) {
        showUsage(argc, argv);
        exit(1);
    }

    string mode = argv[1];
    string filePath = argv[2];

    if (argc > 3) {
        string level = argv[3];
        LOGGING::LogginLevel l = LOGGING::StrToLoggingLevel[level];
        LOG._level = l;
    }

    if (mode == "opencl") {
        runOclBruteForce(filePath);
    } else if (mode == "seq"){
        runSeqBruteForce(filePath);
    } else if (mode == "mpi") {
        runMPIBruteForce(filePath);
    }
    return 0;
}