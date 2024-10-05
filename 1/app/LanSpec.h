#include <iostream>
#include <string>

class LanSpec {
private:
    double speed;  
    std::string macAddress;
public:
    LanSpec(double speed = 0.0,
            const std::string &macAddress = ""
        ) : 
        speed(speed) , 
        macAddress(macAddress)
    {}
    
    ~LanSpec() {}
    
    void Print() const {
        std::cout << "Скорость LAN: " << speed << " Мбит/с"
                  << ", MAC-адрес: " << macAddress << std::endl;
    }

    void Import(std::istream& in) {
        in >> speed
           >> macAddress;
        in.ignore();
    }

    void Export(std::ostream& out) const {
        out << speed << std::endl
            << macAddress;
    }
    bool Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        std::string header;
        std::getline(file, header);
        if (header != "LanSpec") {
            std::cerr << "Неверный формат файла. Ожидается заголовок 'LanSpec'." << std::endl;
            return false;
        }
        file >> speed
             >> macAddress;
        file.ignore();
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << fileName << std::endl;
        return true;
    }

    bool Export(const std::string& fileName) const {
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи: " << fileName << std::endl;
            return false;
        }
        file << "LanSpec" << std::endl
             << speed << std::endl
             << macAddress;
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};