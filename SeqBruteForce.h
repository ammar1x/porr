//
// Created by Ammar on 12/21/16.
//

#ifndef PORR_SEQBRUTEFORCE_H
#define PORR_SEQBRUTEFORCE_H

#include <vector>
#include <string>

#include "KnapsackProblemSolver.h"

#include "CombUtils.h"

/**
 * Brute force
 */
class SeqBruteForceSolver : public KnapsackProblemSolver {


public:

    virtual int solve(KnapsackProblem& problem) override {

        int n = problem.objectsValues.size();
        vector<vector<int>> instances = CombUtils::combination(n);

        long weight = 0;
        long maxVal = 0;
        vector<int> * d;

        for(vector<int> &instance : instances) {

            long tempWeight = 0 ;
            long tempValue  = 0;

            for(int i = 0; i < instance.size(); ++i) {
                if(instance[i] == 0)
                    continue;
                tempWeight += problem.objectsWeights[i];
                tempValue += problem.objectsValues[i];
            }


            if (tempWeight < problem.capacity && tempValue > maxVal) {
                maxVal = tempValue;
                weight = tempWeight;
                d = &instance;
            }

        }
        return maxVal;
    }

};


#endif //PORR_SEQBRUTEFORCE_H
