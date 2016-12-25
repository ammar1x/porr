//
// Created by Ammar on 12/24/16.
//



#include <benchmark/benchmark_api.h>


#include <vector>
#include <string>
#include <tuple>
#include <iostream>

#include "porr.h"

using namespace std;

void benchmark_combinations(benchmark::State& state) {
    while(state.KeepRunning()) {
            CombUtils::combination(state.range(0));
    }
}

void bench_getAllBits(benchmark::State& state) {
    while(state.KeepRunning()) {
        CombUtils::getAllBitsOfLen(state.range(0));
    }
}

OCLBruteForce oclSolver;

cl::Device dev;
cl::Context ctx;



struct ResMem {
    map<int,int> vals;
    string name;
    ResMem(string name) {
        this->name = name;
    }
    void print() {
        cout << "\n-- " << name << endl;
        for(auto it = vals.begin(); it != vals.end(); ++it) {
            cout << " " << it->first << " -> " << it->second << endl;
        }
        cout << "\t--" << endl;
    }
};
ResMem seq("sequential"), ocl("open cl");


void bench_SeqBruteForce(benchmark::State& state) {
    KnapsackProblem kpr = KnapsackProblemFactory::createSimpleProblem(1000, state.range(0));
    SeqBruteForceSolver solver;
    int ans = 0;
    while(state.KeepRunning()) {
        ans = solver.solve(kpr);
    }
    seq.vals[state.range(0)] = ans;
}



void bench_MTBruteForce(benchmark::State& state) {
    KnapsackProblem kpr = KnapsackProblemFactory::createSimpleProblem(1000, state.range(0));
    int ans = 0;
    while(state.KeepRunning()) {
        ans = oclSolver.solve(kpr);
    }

    ocl.vals[state.range(0)] = ans;
}
//BENCHMARK(bench_getAllBits)->RangeMultiplier(2)->Range(2, 20);
//BENCHMARK(benchmark_combinations)->RangeMultiplier(2)->Range(2, 20);


BENCHMARK(bench_SeqBruteForce)->RangeMultiplier(2)->Range(2, 27);
BENCHMARK(bench_MTBruteForce)->RangeMultiplier(2)->Range(2, 27);

void setupOcl() {

    dev = OCLHelper::getAnyDevice();
    ctx = OCLHelper::getContext(dev);
    oclSolver.setupOcl(&dev, &ctx);
}

int main(int argc, char** argv) {
    setupOcl();
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    ocl.print();
    seq.print();
 }