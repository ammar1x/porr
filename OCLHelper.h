//
// Created by Ammar on 12/22/16.
//

#ifndef PORR_OCLHELPER_H
#define PORR_OCLHELPER_H

#include <exception>
#include <vector>
#include <cl.hpp>
#include <string>
#include <sstream>




struct OCLHelper {




    static cl::Buffer createFloatBuffer(cl::Context&context, int n) {
        return cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float)* n);
    }
    static cl::Buffer createIntBuffer(cl::Context &context, int n) {
        return cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int)*n);
    }

    static std::vector<cl::Platform> getPlatforms() {
        std::vector<cl::Platform> all_platforms;
        cl::Platform::get(&all_platforms);
        if(all_platforms.size()==0){
            throw std::runtime_error("No platforms found. Check OpenCL installation!");
        }
        return all_platforms;
    }

    static std::vector<cl::Device> getDevices(cl_device_type device_type = CL_DEVICE_TYPE_ALL) {
        std::vector<cl::Device> allDevs;
        auto platforms = getPlatforms();
        for(auto & platform : platforms) {
            std::vector<cl::Device> devs;
            platform.getDevices(device_type, &devs);
            std::copy(devs.begin(), devs.end(), std::back_inserter(allDevs));
        }

        if(allDevs.size() == 0){
            throw std::runtime_error("No devices found. Check OpenCL installation!");
        }

        return allDevs;
    }

    static std::string getDeviceInfo(cl::Device &device) {
        std::stringstream ss;
        ss<< "CL_DEVICE_NAME : "<<device.getInfo<CL_DEVICE_NAME>()<<"\n";
        ss<< "CL_DEVICE_PLATFORM : "<<device.getInfo<CL_DEVICE_PLATFORM>()<<"\n";
        ss<< "CL_DEVICE_TYPE : "<<device.getInfo<CL_DEVICE_TYPE>()<<"\n";
        return ss.str();
    }



};


#endif //PORR_OCLHELPER_H