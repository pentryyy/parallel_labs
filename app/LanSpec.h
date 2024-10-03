#include <iostream>
#include <string>

class LanSpec {
private:
    double speed;  
    std::string macAdress;
public:
    LanSpec(double speed = 0.0,
            const std::string &macAdress = ""
        ) : 
        speed(speed) , 
        macAdress(macAdress)
    {}
    
    ~LanSpec() {}
    
    void Print() const {
        std::cout << "LAN Speed: " << speed << " Mbps"
                  << ", Mac Adress: " << macAdress << std::endl;
    }

    void Import(std::istream &in) {
        in >> speed
           >> macAdress;
    }

    void Export(std::ostream &out) const {
        out << speed << " "
            << macAdress << std::endl;
    }
};