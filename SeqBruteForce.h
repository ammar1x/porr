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

    virtual long solve(const KnapsackProblem& problem) override {

        int n = problem.objectsValues.size();

        long weight = 0;
        long maxVal = 0;

        int m = 1 << n; // number of possible solutions
        for(int i = 0; i < m; ++i) {
            long tmpWeight = 0;
            long tmpValue = 0;

            int num = i;
            int j = 0;
            do {
                bool take = (num & 1) == 1;
                if (take) {
                    tmpWeight += problem.objectsWeights[j];
                    tmpValue += problem.objectsValues[j];
                }
                j++;
            } while(num >>= 1);

            if(tmpWeight <= problem.capacity && tmpValue > maxVal) {
                maxVal = tmpValue;
            }
        }
        return maxVal;
    }

};


#endif //PORR_SEQBRUTEFORCE_H
