#include "InputOutput.h"

class LanSpec : public InputOutput {
private:
    double speed;  
    std::string macAddress;
protected:
    std::string GetClassHeader() const override { return "LanSpec"; }
public:
    LanSpec(double speed = 0.0,
            const std::string &macAddress = ""
        ) : 
        speed(speed) , 
        macAddress(macAddress)
    {}
    
    ~LanSpec() {}
    
    double GetSpeed() const { return speed; }
    std::string GetMacAddress() const { return macAddress; }

    void Print() const {
        std::cout << "Скорость LAN: " << speed << " Мбит/с"
                  << ", MAC-адрес: " << macAddress << std::endl;
    }

    bool Import(std::istream& in) override {
        if (!CheckHeaderName(in)) { return false; }
        in >> speed
           >> macAddress;
        in.ignore();
        return true;
    }

    void Export(std::ostream& out) const override {
        out << GetClassHeader() << std::endl
            << speed << std::endl
            << macAddress;
    }
};