//
// Created by Ammar on 12/22/16.
//

#ifndef PORR_OPENCLBRUTEFORCE_H_H
#define PORR_OPENCLBRUTEFORCE_H_H

#include <cl.hpp>
#include <fstream>
#include <streambuf>
#include "KnapsackProblem.h"
#include "OCLHelper.h"
#include <glog/logging.h>


#include "KnapsackProblemSolver.h"

class OCLBruteForce : public KnapsackProblemSolver {
public:


    OCLBruteForce() :
            /**
             * Path is relative to binary file.
             */
            kernelPath("./conf/KnapsackBruteSolver.cl"){

    }


    void setContext(cl::Context* context) {
        this->context = context;
    }

    void setDevice(cl::Device* device) {
        this->device = device;
    }

    void setupOcl(cl::Device* dev, cl::Context* ctx) {
        this->device = dev;
        this->context = ctx;
        buildProgram();
        LOG(INFO) << OCLHelper::getDeviceInfo(*this->device) << endl;

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
        if(program.build({*device}) != CL_SUCCESS){
            std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(*device)<<"\n";
            exit(1);
        }
        this->program = program;
    }

    virtual long solve(const KnapsackProblem& problem) override {


        int n = problem.objectsValues.size();


        int count = std::min(20, n);
        int m = 1 << count;

        cl::Buffer instancesBuffer = OCLHelper::createCharBuffer(*context, 1);

        cl::Buffer valuesBuffer = OCLHelper::createIntBuffer(*context, n);
        cl::Buffer weightsBuffer = OCLHelper::createIntBuffer(*context, n);
        cl::Buffer instancesOutBuffer = OCLHelper::createIntBuffer(*context, m);

        cl::CommandQueue queue(*context,*device);

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

        int *out = new int[m];
        int maxVal = 0;


        for(int i = 0, offset = 0; i <= (1 << n); i += m) {

            cl::Event event;
            queue.enqueueNDRangeKernel(kernel,
                                       cl::NDRange(i),
                                       cl::NDRange(m),
                                       cl::NullRange,
                                       nullptr, &event);


            event.wait();
            queue.enqueueReadBuffer(instancesOutBuffer,CL_TRUE,0, sizeof(int)*m, out);
            maxVal = std::max(max(out, m), maxVal);

            if (i == 0) {
                i = m;
            }

            offset = i;
        }


        delete [] out;

        return maxVal;

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
