#include "InputOutput.h"

class CpuSpec : public InputOutput {
private:
    std::string model;
    int pcoreCount;
    int ecoreCount;
    bool hasEcore;
    bool hasMultithreading;
    double pcoreFrequency;
    double ecoreFrequency;
    double uncoreFrequency;
public:
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
    
    ~CpuSpec() {}

    std::string GetClassHeader() const override { return "CpuSpec"; }

    void Print() const { 
        std::cout << "Модель процессора: " << model
                << ", Количество производительных ядер: " << pcoreCount
                << ", Количество эффективных ядер: " << (ecoreCount ? std::to_string(ecoreCount) : "Нет")
                << ", Наличие E-core: " << (hasEcore ? "Да" : "Нет")
                << ", Наличие многопоточности: " << (hasMultithreading ? "Да" : "Нет")
                << ", Частота производительных ядер: " << pcoreFrequency << " МГц, "
                << (hasEcore ? "Частота эффективных ядер: " + std::to_string(ecoreFrequency) + " МГц" : "Частота эффективных ядер: - МГц")
                << ", Частота межъядерной шины: " << uncoreFrequency << " МГц" << std::endl;
    }

    bool Import(std::istream& in) override {
        if (!CheckHeaderName(in)) { return false; }
        std::getline(in, model);
        in >> pcoreCount
           >> ecoreCount
           >> hasEcore
           >> hasMultithreading
           >> pcoreFrequency
           >> ecoreFrequency
           >> uncoreFrequency;
        in.ignore();
        return true;
    }

    void Export(std::ostream& out) const override {
        out << GetClassHeader() << std::endl
            << model << std::endl
            << pcoreCount << std::endl
            << ecoreCount << std::endl
            << hasEcore << std::endl
            << hasMultithreading << std::endl
            << pcoreFrequency << std::endl
            << ecoreFrequency << std::endl
            << uncoreFrequency;
    }
};