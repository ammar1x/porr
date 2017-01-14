//
// Created by Ammar on 12/21/16.
//

#ifndef PORR_SEQBRUTEFORCE_H
#define PORR_SEQBRUTEFORCE_H

#include <vector>
#include <string>

#include "KnapsackProblemSolver.h"
#include "utils.h"

using namespace LOGGING;

/**
 * Brute force
 */
class SeqBruteForceSolver : public KnapsackProblemSolver {


public:

    virtual long solve(const KnapsackProblem& problem) override {

        int64_t n = problem.objectsValues.size();

        int64_t weight = 0;
        int64_t maxVal = 0;

        int64_t one = 1;
        int64_t m = one << n; // number of possible solutions

        LogStream::global().DEBUG() << "SeqBruteForceSolver: " << "solutions size = " << m << endl;


        for(int64_t i = 0; i < m; ++i) {
            int64_t tmpWeight = 0;
            int64_t tmpValue = 0;

            int64_t num = i;
            int64_t j = 0;
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


    virtual long solve(const KnapsackProblem& problem, int64_t start, int64_t stop)  {

        int n = problem.objectsValues.size();

        long weight = 0;
        long maxVal = 0;

        for(int64_t i = start; i < stop; ++i) {
            int64_t tmpWeight = 0;
            int64_t tmpValue = 0;

            int64_t num = i;
            int64_t j = 0;
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
