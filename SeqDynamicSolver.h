//
// Created by Ammar on 12/21/16.
//

#ifndef PORR_KNAPSACKPROBLEMDYNAMICSOLVER_H
#define PORR_KNAPSACKPROBLEMDYNAMICSOLVER_H


#include "KnapsackProblem.h"

/**
 *
 */
class SeqDynamicSolver {
public:
    int solve(KnapsackProblem& knapsackProblem) {

        int n = knapsackProblem.objectsValues.size();
        int W = knapsackProblem.capacity;
        vector<int> &w = knapsackProblem.objectsWeights;
        vector<int> &v = knapsackProblem.objectsValues;

        vector<vector<int>> m;
        m.push_back(vector<int>());
        for(int j = 0; j <= W; ++j) {
            m[0].push_back(0);
        }


        for(int i = 1; i <= n; ++i) {
            m.push_back(vector<int>());
            for(int j = 0; j <= W; ++j) {
                int val;
                int cw = w[i-1];
                int cv = v[i-1];
                if (cw > j) {
                    m[i].push_back(m[i-1][j]);
                } else {
                    m[i].push_back(
                            std::max(m[i-1][j], m[i-1][j-cw] + cv));
                }
            }
        }
        return m[n][W];
    }

};


#endif //PORR_KNAPSACKPROBLEMDYNAMICSOLVER_H
