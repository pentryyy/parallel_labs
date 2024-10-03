#include <iostream>
#include <string>

class RamSpec {
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

    void Print() const {
        std::cout << "RAM Capacity: " << capacity << " MB"
                  << ", Frequency: " << frequency << " MHz"
                  << ", Type: " << type << std::endl;
    }

    void Import(std::istream &in) {
        in >> capacity
           >> frequency
           >> type;
    }

    void Export(std::ostream &out) const {
        out << capacity << " "
            << frequency << " "
            << type << std::endl;
    }
};