//
// Created by Ammar on 12/20/16.
//

#ifndef PORR_CONFIGURATION_H
#define PORR_CONFIGURATION_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <json.hpp>


using namespace std;

/**
 * Reads data file.
 */
class Configuration {

public:

    Configuration() {}

    static Configuration fromFile(string fileName) {


        fstream f;

        f.open(fileName);
        if (!f.is_open()) {
            throw std::runtime_error("Couldnt open file " + fileName + " for reading");
        }

        Configuration c;
        c.knapsacksCount = nextInt(f);
        c.objectsCount = nextInt(f);
        nextInts(f, c.objectsValues, c.objectsCount);
        nextInts(f, c.knapsacksCapacities, c.knapsacksCount);
        for(int i = 0; i < c.knapsacksCount; ++i) {
            vector<int> constraints;
            nextInts(f, constraints, c.objectsCount);
            c.constraints.push_back(constraints);
        }
        c.optimalValue = nextInt(f);
        c.nameOfTheGame = nextToken(f);

        return c;
    }



    static void skipLine(fstream &f) {
        int c;
        while(((c = f.get()) != '\n')  && c != '\r')
            ;
    }



    static void nextInts(fstream& f, vector<int> &dst, int limit) {
        int i = 0;
        while(i < limit) {
            dst.push_back(nextInt(f));
            i++;
        }
    }

    static string nextToken(fstream& f) {

        if (f.eof()) {
            return "";
        }

        string token;
        f >> token;
        if (token[0] == '/') {
            skipLine(f);
            return nextToken(f);
        }
        return token;
    }

    static int nextInt(fstream &f) {

        return atoi(nextToken(f).c_str());
    }




     /** For easier deubugging **/
     nlohmann::json toJson() {
        using json = nlohmann::json;
        json j;
        j["nameOfTheGame"] = nameOfTheGame;
        j["optimalValue"] = optimalValue;
        j["knapsacksCount"] = knapsacksCount;
        j["objectsCount"] = objectsCount;
        j["objectsValues"] = objectsValues;
        j["knapsacksCapacities"] = knapsacksCapacities;
        j["constraints"] = constraints;
        return j;
    }


    static Configuration fromJsonFile(std::string path) {
        std::ifstream i(path);
        nlohmann::json j;
        i >> j;
        return Configuration::fromJson(j);
    }

    static Configuration fromJson(std::string f) {
        return Configuration::fromJson(nlohmann::json::parse(f));
    }

    static Configuration fromJson(nlohmann::json j) {
        Configuration c;

        c.nameOfTheGame = j["nameOfTheGame"];
        c.optimalValue = j["optimalValue"];
        c.knapsacksCount = j["knapsacksCount"];
        c.objectsCount = j["objectsCount"];

        for(auto el : j["objectsValues"]) {
            c.objectsValues.push_back(el);
        }

        for(int  el: j["knapsacksCapacities"]) {
            c.knapsacksCapacities.push_back(el);
        }

        int i = 0;
        for(auto  v : j["constraints"]) {
            c.constraints.push_back(std::vector<int>());
            for(int  e : v) {
                c.constraints[i].push_back(e);
            }
            i++;
        }


         return c;
    }

    /*
     * get only basic information
     */
    nlohmann::json toBasicJson() {
        using json = nlohmann::json;
        json j;
        j["nameOfTheGame"] = nameOfTheGame;
        j["optimalValue"] = optimalValue;
        j["knapsacksCount"] = knapsacksCount;
        j["objectsCount"] = objectsCount;
        return j;
    }

    string nameOfTheGame = "";
    long optimalValue = -1;
    int knapsacksCount = -1;
    int objectsCount = -1;
    vector<int> objectsValues;
    vector<int> knapsacksCapacities;
    // first index = knapsack
    // second index = constraint of object
    vector<vector<int>> constraints;


};


#endif //PORR_CONFIGURATION_H
