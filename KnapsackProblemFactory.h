//
// Created by Ammar on 12/24/16.
//

#ifndef PORR_KNAPSACKPROBLEMFACTORY_H
#define PORR_KNAPSACKPROBLEMFACTORY_H

#include "KnapsackProblem.h"
#include <vector>


struct DataFactory {
    static std::vector<int> range(int start, int stop, int step=1) {
        std::vector<int> ret;
        for(int i = start; i < stop; i+=step) {
            ret.push_back(i);
        }
        return ret;
    }
};

struct KnapsackProblemFactory {

    static KnapsackProblem createSimpleProblem(int capacity, int objectsCount) {
        KnapsackProblem knp;
        knp.capacity = capacity;
        knp.objectsValues = DataFactory::range(1, objectsCount+1);
        knp.objectsWeights = DataFactory::range(1, objectsCount+1);
        return knp;
    }

};


#endif //PORR_KNAPSACKPROBLEMFACTORY_H
