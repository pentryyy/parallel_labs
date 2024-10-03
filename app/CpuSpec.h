#include <iostream>
#include <string>

class CpuSpec {
public:
    std::string model;
    int pcoreCount;
    int ecoreCount;
    bool hasEcore;
    bool hasMultithreading;
    double pcoreFrequency;
    double ecoreFrequency;
    double uncoreFrequency;

    CpuSpec(const std::string &model = "", 
            int pcoreCount = 0,
            int ecoreCount = 0,
            bool hasEcore = false,
            bool hasMultithreading = false,
            double pcoreFrequency = 0.0,
            double ecoreFrequency = 0.0,
            double uncoreFrequency = 0.0
        ) :
        model(model), 
        pcoreCount(pcoreCount), 
        ecoreCount(ecoreCount),
        hasEcore(hasEcore),
        hasMultithreading(hasMultithreading),
        pcoreFrequency(pcoreFrequency),
        ecoreFrequency(ecoreFrequency),
        uncoreFrequency(uncoreFrequency)
    {}

    void Print() const {
        std::cout << "CPU Model: " << model
                  << ", Performance Cores: " << pcoreCount
                  << ", Efficiency Cores: " << (ecoreCount ? std::to_string(ecoreCount) : "No")
                  << ", Has E-core: " << (hasEcore ? "Yes" : "No")
                  << ", Has Multithreading: " << (hasMultithreading ? "Yes" : "No")
                  << ", Performance Core Frequency: " << pcoreFrequency << " MHz, "
                  << (hasEcore ? "Efficiency Core Frequency: " + std::to_string(ecoreFrequency) + " MHz" : "- Mhz")
                  << ", Uncore Frequency: " << uncoreFrequency << " MHz" << std::endl;
    }

    void Import(std::istream &in) {
        in >> model
           >> pcoreCount
           >> ecoreCount
           >> hasEcore
           >> hasMultithreading
           >> pcoreFrequency
           >> ecoreFrequency
           >> uncoreFrequency;
    }

    void Export(std::ostream &out) const {
        out << model << " "
            << pcoreCount << " "
            << ecoreCount << " "
            << hasEcore << " "
            << hasMultithreading << " "
            << pcoreFrequency << " "
            << ecoreFrequency << " "
            << uncoreFrequency << std::endl;
    }
};
