//
// Created by Ammar on 12/21/16.
//


#include <gtest/gtest.h>
#include "porr.h"

#include <vector>
#include <string>

using  namespace std;



TEST(TestGTest, NoWhere) {
    EXPECT_EQ(0, 0);
}

TEST(TestConfiguration, conf) {
    // nothing
}

TEST(TestBruteForce, bruteOne) {
    KnapsackProblem pr;
    pr.capacity = 900;
    pr.objectsWeights = {4, 6, 5, 5, 5, 5, 5,5 ,5, 5,5};
    pr.objectsValues = {4, 6, 5, 5, 5, 5, 5,5 ,5, 5,5};
    SeqBruteForceSolver solver;
    int sol = solver.solve(pr);
    cout << sol << endl;
//    EXPECT_EQ(sol, 12);
}


TEST(TestBruteForce, bruteTwo) {
    KnapsackProblem pr;
    pr.capacity = 5;
    pr.objectsWeights = {40};
    pr.objectsValues = {12};
    SeqBruteForceSolver solver;
    int sol = solver.solve(pr);
    EXPECT_EQ(sol, 0);
}

int main(int argc, char*argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}