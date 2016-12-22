//
// Created by Ammar on 12/22/16.
//

#include <iostream>
#include <vector>
#include <string>
#include <cl.hpp>
#include <gtest/gtest.h>
#include <exception>
#include "OCLHelper.h"

/**
 * test opencl features
 */

TEST(TestOpenCL, findPlatforms) {
    using namespace std;

    vector<cl::Platform> platforms = OCLHelper::getPlatforms();
    cl::Platform default_platform = platforms[0];
    std::cout << "Using platform: "<< default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";
}

TEST(TestOpenCL, findDevices) {
   using namespace std;
    vector<cl::Device> devs = OCLHelper::getDevices();
    for(cl::Device& device : devs) {

        cout<< "CL_DEVICE_NAME : "<<device.getInfo<CL_DEVICE_NAME>()<<"\n";
        cout<< "CL_DEVICE_PLATFORM : "<<device.getInfo<CL_DEVICE_PLATFORM>()<<"\n";
        cout<< "CL_DEVICE_TYPE : "<<device.getInfo<CL_DEVICE_TYPE>()<<"\n";
        cout << "\n";

    }
}

int main(int argc, char*argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}