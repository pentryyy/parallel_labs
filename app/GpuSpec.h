#include <iostream>
#include <string>

class GpuSpec {
public:
    std::string model;
    int memorySize;
    int coreCount;
    double coreFrequency;
    double vramFrequency;

    GpuSpec(const std::string &model = "", 
            int memorySize = 0, 
            int coreCount = 0, 
            double coreFrequency = 0.0, 
            double vramFrequency = 0.0
        ) : 
        model(model),
        memorySize(memorySize), 
        coreCount(coreCount), 
        coreFrequency(coreFrequency), 
        vramFrequency(vramFrequency)
    {}

    void Print() const {
        std::cout << "GPU Model: " << model
                  << ", Memory Size: " << memorySize << " MB"
                  << ", Core Count: " << coreCount
                  << ", Core Frequency: " << coreFrequency << " MHz"
                  << ", VRAM Frequency: " << vramFrequency << " MHz" << std::endl;
    }

    void Import(std::istream &in) {
        in >> model 
           >> memorySize
           >> coreCount
           >> coreFrequency
           >> vramFrequency;
    }

    void Export(std::ostream &out) const {
        out << model << " "
            << memorySize << " "
            << coreCount << " " 
            << coreFrequency << " "
            << vramFrequency << std::endl;
    }
};
