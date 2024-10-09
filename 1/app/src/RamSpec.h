#include "InputOutput.h"

class RamSpec : public InputOutput {
private:
    int capacity;
    double frequency;
    std::string type;
public:
    RamSpec(int capacity = 0, 
            double frequency = 0.0,
            const std::string& type = ""
        ) :
        capacity(capacity), 
        frequency(frequency), 
        type(type) 
    {}

    ~RamSpec() {}

    std::string GetClassHeader() const override { return "RamSpec"; }

    void Print() const {
        std::cout << "Объем ОЗУ: " << capacity << " МБ"
                  << ", Частота: " << frequency << " МГц"
                  << ", Тип: " << type << std::endl;
    }

    bool Import(std::istream& in) override {
        if (!CheckHeaderName(in)) { return false; }
        in >> capacity
           >> frequency
           >> type;
        in.ignore();
        return true;
    }

    void Export(std::ostream& out) const override {
        out << GetClassHeader() << std::endl
            << capacity << std::endl
            << frequency << std::endl
            << type;
    }
};