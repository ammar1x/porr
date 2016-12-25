//
// Created by Ammar on 12/24/16.
//

#ifndef PORR_ASYNCPROBLEMSOLVER_H
#define PORR_ASYNCPROBLEMSOLVER_H

#include "KnapsackProblem.h"
#include "KnapsackProblemSolver.h"
#include "CombUtils.h"
#include <tuple>
#include <future>
#include <iostream>
#include <functional>

struct MTKnapsackProblemSolver : public KnapsackProblemSolver {


    long solve(const KnapsackProblem& problem) override {
        using namespace std;

        int n = problem.objectsValues.size();
        int cap = problem.capacity;
        int maxVal = 0;

        vector<vector<int>> instances = CombUtils::combination(n);
        using res_type = tuple<int,int>;
        vector<shared_future<res_type>> results;
        for(int i = 0; i < instances.size(); ++i) {
            shared_future<res_type> t = std::async([i, &instances, &problem](){
                return MTKnapsackProblemSolver::comp(instances[i], problem);
            });
            results.push_back(t);
        }

        for(int i = 0; i < results.size(); ++i) {
            auto res = results[i].get();
            int weight = -1, value = -1;
            std::tie (weight, value) = res;
            if (weight <= cap && value > maxVal) {

                maxVal = value;
            }
        }

        return maxVal;
    }

    static tuple<int,int> comp(std::vector<int>& instance, const KnapsackProblem& problem) {

        long tempWeight = 0 ;
        long tempValue  = 0;

        for(int i = 0; i < instance.size(); ++i) {
            if(instance[i] == 0)
                continue;
            tempWeight += problem.objectsWeights[i];
            tempValue += problem.objectsValues[i];
        }

        return std::make_tuple(tempWeight, tempValue);
    }

};


#endif //PORR_ASYNCPROBLEMSOLVER_H
