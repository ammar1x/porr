//
// Created by Ammar on 12/22/16.
//

#ifndef PORR_OPENCLBRUTEFORCE_H_H
#define PORR_OPENCLBRUTEFORCE_H_H

#include <cl.hpp>
#include <fstream>
#include <streambuf>
#include "KnapsackProblem.h"
#include "CombUtils.h"
#include "OCLHelper.h"


#include "KnapsackProblemSolver.h"

class OCLBruteForce : public KnapsackProblemSolver {
public:


    OCLBruteForce() :
            kernelPath("./conf/KnapsackBruteSolver.cl"){

    }


    void setContext(cl::Context* context) {
        this->context = context;
    }

    void setDevice(cl::Device* device) {
        this->device = device;
    }


    void setKernelPath(std::string kernelPath) {
        this->kernelPath = kernelPath;
    }

    std::string readKernel(std::string path) {
        std::ifstream t(path);
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        return str;

    }

    void buildProgram() {
        std::string kernel = readKernel(kernelPath);
        cl::Program::Sources sources;
        sources.push_back({kernel.c_str(),kernel.length()});

        cl::Program program(*context, sources);
        if(program.build({*device})!=CL_SUCCESS){
            std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(*device)<<"\n";
            exit(1);
        }
        this->program = program;
    }

    virtual int solve(KnapsackProblem& problem) override {

        int n = problem.objectsValues.size();

        vector<vector<int>> instances = CombUtils::combination(n);
        int m = instances.size();
        // for each instance spawn a new worker


        cl::Buffer instancesBuffer = OCLHelper::createIntBuffer(*context, n * m);
        cl::Buffer valuesBuffer = OCLHelper::createIntBuffer(*context, n);
        cl::Buffer weightsBuffer = OCLHelper::createIntBuffer(*context, n);
        cl::Buffer instancesOutBuffer = OCLHelper::createIntBuffer(*context, m);


        cl::CommandQueue queue(*context,*device);

        int* h = CombUtils::asArray(instances);

        queue.enqueueWriteBuffer(instancesBuffer,
                                 CL_TRUE, 0,
                                 sizeof(int)*n*m, h);

        queue.enqueueWriteBuffer(valuesBuffer,
                                 CL_TRUE,0,
                                 sizeof(int)*n,
                                 problem.objectsValues.data());

        queue.enqueueWriteBuffer(weightsBuffer, CL_TRUE,
                                 0, sizeof(int)*n,
                                 problem.objectsWeights.data());


        cl::Kernel kernel(program,"solve");
        kernel.setArg(0, instancesBuffer);
        kernel.setArg(1, valuesBuffer);
        kernel.setArg(2, weightsBuffer);
        kernel.setArg(3, instancesOutBuffer);
        kernel.setArg(4, n);
        kernel.setArg(5, problem.capacity);


        cl::Event event;
        queue.enqueueNDRangeKernel(kernel,
                                   cl::NullRange,
                                   cl::NDRange(m),
                                   cl::NullRange,
                                   nullptr, &event);

        int *out = new int[m];

        event.wait();
        queue.enqueueReadBuffer(instancesOutBuffer,CL_TRUE,0, sizeof(int)*m, out);
        int val = max(out, m);

        delete [] out;
        delete [] h;

        return val;

    }

    int max(int* v, int n) {
        int re = v[0];
        for(int i = 0; i < n; ++i) {
            re = std::max(re, v[i]);
        }
        return re;
    }

private:
    std::string kernelPath;
    cl::Context* context;
    cl::Device* device;
    cl::Program program;


};
#endif //PORR_OPENCLBRUTEFORCE_H_H
