//
// Created by Ammar on 12/22/16.
//

#ifndef PORR_KNAPSACKPROBLEMSOLVER_H
#define PORR_KNAPSACKPROBLEMSOLVER_H

#include "KnapsackProblem.h"

class KnapsackProblemSolver {

public:
    virtual ~KnapsackProblemSolver() {};
    virtual int solve(KnapsackProblem& problem) = 0;
};


#endif //PORR_KNAPSACKPROBLEMSOLVER_H
