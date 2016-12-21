//
// Created by Ammar on 12/21/16.
//

#ifndef PORR_COMBUTILS_H
#define PORR_COMBUTILS_H

#include <vector>
#include <string>
using namespace std;


class CombUtils {


public:

    static void toString(vector<int>& l) {
        cout << "[";
        for(int d : l) {
            cout << d << ",";
        }
        cout << "]";
        cout << endl;

    }
    static void toString(vector<vector<int>> &s) {
        cout << "[" << endl;
        for(vector<int>& l : s) {
            toString(l);
        }
        cout << "]";
    }

    static vector<vector<int>> combination(int n) {
        if (n == 0) {
            vector<vector<int>> d;
            return d;
        }

        else if (n == 1) {
            vector<int> o1 = {1};
            vector<int> o2 = {0};
            vector<vector<int>> out = {o1, o2};
            return out;
        } else {
            vector<vector<int>> out;
            for(vector<int> df : combination(n-1)) {

                vector<int> o1 = df;
                vector<int> o2 = df;

                o1.insert(o1.begin(), 0);
                o2.insert(o2.begin(), 1);

                out.push_back(o1);
                out.push_back(o2);
            }
            return out;
        }
    }

};


#endif //PORR_COMBUTILS_H
