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
#include <vector>

static LOGGING::LogStream& LOG = LOGGING::LogStream::global(LOGGING::info);

using LOGGING::endl;

struct Stats {
    double minMS;
    double maxMS;
    double avgMS;
    std::vector<double> points;
    void calcStats() {
        minMS = 999999999, maxMS = 0, avgMS = 0;
        for(int i = 0; i < points.size(); ++i) {
            avgMS += points[i];
            if (maxMS < points[i]) {
                maxMS = points[i];
            }
            if(minMS > points[i]) {
                minMS = points[i];
            }
        }
        avgMS /= points.size();
    }

    std::string toString() {
        std::stringstream ss;
        ss << "#reps = " << points.size() << std::endl;
        ss << "min = " << minMS << " ms " << std::endl;
        ss << "max = " << maxMS << " ms " << std::endl;
        ss << "avg = " << avgMS << " ms "<< std::endl;
        return ss.str();
    }

};

int runMPIBruteForce(string filePath, int rep) {
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
        LOG.DEBUG() <<  "M: broadcasting configuration" << endl;


        Stats stats;
        int64_t maxVal = -1;

        for(int i = 0; i < rep; ++i) {

            Stopwatch<> sw;
            sw.start();




            MPIHelper::broadcast(cjson, MASTERS_RANK);
            KnapsackProblem p = getKnapsackProblems(c)[0];


            int64_t workersSize = size - 1;
            int64_t one = 1;
            int64_t solutions = one << (p.objectsValues.size());
            int64_t perWorker = solutions / workersSize;

            if (i == 0) {
                LOG.INFO() << "number of schedulers = " << 1 << endl;
                LOG.INFO() << "number of workers = " << size - 1 << endl;
                LOG.INFO() << "total search space size = " << solutions << endl;
                LOG.INFO() << "search space per worker = " << perWorker << endl;
            }

            vector<int64_t> res(size, -1);
            int64_t fake = -1;
            MPI_Gather(&fake, 1, MPI_INT64_T, res.data(), 1, MPI_INT64_T, MASTERS_RANK, MPI_COMM_WORLD);

            for_each(res.begin(), res.end(), [&maxVal](int64_t a) {
                maxVal = std::max(a, maxVal);
            });

            stats.points.push_back(sw.elapsed() / 1000.0);
        }

        stats.calcStats();


        LOG.INFO() << "M: optimal value = " << maxVal << endl;
        LOG.INFO() << stats.toString() << endl;

    } else {
        for(int i = 0; i < rep; ++i) {


            LOG.DEBUG() << "S(" << rank << ")" << " waiting for configuration" << endl;
            std::string cjson = MPIHelper::broadcastRecv<std::string>(MASTERS_RANK);
            LOG.DEBUG() << "S(" << rank << ")" << " Received configuration";


            Configuration c = Configuration::fromJson(cjson);
            KnapsackProblem p = getKnapsackProblems(c)[0]; // get the first one

            int workersSize = size - 1;
            int64_t one = 1;
            int64_t solutions = one << (p.objectsValues.size());
            int64_t perWorker = solutions / workersSize;

            int64_t from = (rank - 1) * perWorker;
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
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPIHelper::finalize();
    return 0;
}



int runOclBruteForce(string filePath, int rep) {

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

    Stats s;
    int64_t sol;
    for(int i = 0; i < rep; ++i) {
        Stopwatch<> sw;
        sw.start();
        sol = solver.solve(problems[0]);
        s.points.push_back(sw.elapsed() * 1.0 / 1000.0);
    }

    s.calcStats();

    LOG.INFO() << "Optimal value = " << sol << endl;
    LOG.INFO() << s.toString() << endl;

    return 0;
}

int runSeqBruteForce(string filePath, int rep) {

    LOG.DEBUG() << "running sequential version " << endl;
    LOG.DEBUG() << "file path " << filePath << endl;

    Configuration conf = Configuration::fromFile(filePath);
    LOG.INFO() << conf.toBasicJson().dump(2) << endl;
    vector<KnapsackProblem> problems = getKnapsackProblems(conf);
    SeqBruteForceSolver solver;

    Stopwatch<> sw;
    sw.start();
    KnapsackProblem kp = problems[0];

    Stats s;
    int64_t sol = 0;
    for(int i = 0; i < rep; ++i) {
        sw.start();
        sol = solver.solve(problems[0]);
        s.points.push_back(sw.elapsed() * 1.0 / 1000.0);
    }
    s.calcStats();

    LOG.INFO() << "Optimal value = " << sol << endl;
    LOG.INFO() << s.toString() << endl;

    return 0;
}

void showUsage(int argc, char*argv[]) {
    LOG.INFO() << "usage: " << endl;
    LOG.INFO() << argv[0] << " <<mode>>  <<filePath>> [rep=1] [logLevel=info]" << endl;
    LOG.INFO() << "mode = {opencl, seq, mpi}" << endl;
    LOG.INFO() << "filePath = path to problem file" << endl;
    LOG.INFO() << "rep = number of reps  " << endl;
    LOG.INFO() << "logLevel = debug, info" << endl;
}
int main(int argc, char* argv[]) {

    if (argc < 3) {
        showUsage(argc, argv);
        exit(1);
    }

    int rep = 1;
    string mode = argv[1];
    string filePath = argv[2];

    if (argc > 3) {
        rep = std::max(atoi(argv[3]), 1);
    }

    if (argc > 4) {
        string level = argv[3];
        LOGGING::LogginLevel l = LOGGING::StrToLoggingLevel[level];
        LOG._level = l;
    }



    if (mode == "opencl") {
        runOclBruteForce(filePath, rep);
    } else if (mode == "seq"){
        runSeqBruteForce(filePath, rep);
    } else if (mode == "mpi") {
        runMPIBruteForce(filePath, rep);
    }
    return 0;
}