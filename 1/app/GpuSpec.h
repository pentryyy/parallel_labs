#include "InputOutput.h"

class GpuSpec : public InputOutput {
private:
    std::string model;
    int memorySize;
    int coreCount;
    double coreFrequency;
    double vramFrequency;    
public:
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

    ~GpuSpec() {}
    
    std::string GetClassHeader() const override { return "GpuSpec"; }

    void Print() const { 
        std::cout << "Модель видеокарты: " << model
                  << ", Объем памяти: " << memorySize << " МБ"
                  << ", Количество ядер: " << coreCount
                  << ", Частота ядер: " << coreFrequency << " МГц"
                  << ", Частота VRAM: " << vramFrequency << " МГц" << std::endl;
    }

    bool Import(std::istream& in) override {
        if (!CheckHeaderName(in)) { return false; }
        std::getline(in, model);
        in >> memorySize 
           >> coreCount 
           >> coreFrequency 
           >> vramFrequency;
        in.ignore();
        return true;
    }

    void Export(std::ostream& out) const override {
        out << GetClassHeader() << std::endl
            << model << std::endl
            << memorySize << std::endl
            << coreCount << std::endl
            << coreFrequency << std::endl
            << vramFrequency;
    }
};