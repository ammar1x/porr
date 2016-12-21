//
// Created by Ammar on 12/21/16.
//

#ifndef PORR_KNAPSACKPROBLEM_H
#define PORR_KNAPSACKPROBLEM_H

#include <vector>
#include <string>
#include "Configuration.h"

using namespace std;
/**
 * Defines 0/1 KnapsackProblem parameters
 */
struct KnapsackProblem {

    vector<int> objectsValues;
    vector<int> objectsWeights;
    int capacity;

};

vector<KnapsackProblem> getKnapsackProblems(Configuration& conf) {
    vector<KnapsackProblem> knapsackProblems;
    for(int i = 0; i < conf.knapsacksCount; ++i) {
        KnapsackProblem knapsackProblem;
        knapsackProblem.capacity = conf.knapsacksCapacities[i];
        knapsackProblem.objectsWeights = conf.constraints[i];
        knapsackProblem.objectsValues = conf.objectsValues;
        knapsackProblems.push_back(knapsackProblem);
    }
    return knapsackProblems;
}


#endif //PORR_KNAPSACKPROBLEM_H
