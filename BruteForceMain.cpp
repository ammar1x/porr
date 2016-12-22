#include <iostream>

#include "Configuration.h"
#include "KnapsackProblem.h"
#include "SeqBruteForce.h"
#include "OCLBruteForce.h"
#include "OCLHelper.h"

using namespace std;



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
    cout << "mode = {opencl, seq}" << endl;
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
    } else {
        runSeqBruteForce(filePath);
    }
    return 0;
}