#include <iostream>

#include "Configuration.h"
#include "KnapsackProblem.h"
#include "SeqDynamicSolver.h"
#include "SeqBruteForce.h"
#include <vector>

using namespace std;



int main() {
    string filePath = "/Users/ammar/Dropbox/porr/data/pet2.dat";
    Configuration conf = Configuration::fromFile(filePath);
    vector<KnapsackProblem> problems = getKnapsackProblems(conf);

    SeqBruteForceSolver bruteForceSolver;
    for(KnapsackProblem& problem : problems) {
        cout << bruteForceSolver.solve(problem) << endl;
    }
}

int main1() {
    string filePath = "/Users/ammar/Dropbox/porr/data/pb7.dat";
    Configuration conf = Configuration::fromFile(filePath);
    vector<KnapsackProblem> problems = getKnapsackProblems(conf);

    SeqDynamicSolver dynamicSolver;
    for(KnapsackProblem& problem : problems) {
        cout << dynamicSolver.solve(problem) << endl;
    }

    return 0;
}