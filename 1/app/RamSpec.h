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
        std::cout << "Объем ОЗУ: " << capacity << " МБ"
                  << ", Частота: " << frequency << " МГц"
                  << ", Тип: " << type << std::endl;
    }

    void Import(std::istream& in) {
        in >> capacity
           >> frequency
           >> type;
        in.ignore();
    }

    void Export(std::ostream& out) const {
        out << capacity << std::endl
            << frequency << std::endl
            << type;
    }

    bool Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        std::string header;
        std::getline(file, header);
        if (header != "RamSpec") {
            std::cerr << "Неверный формат файла. Ожидается заголовок 'RamSpec'." << std::endl;
            return false;
        }
        file >> capacity
             >> frequency
             >> type;
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
        file << "RamSpec" << std::endl
             << capacity << std::endl
             << frequency << std::endl
             << type;
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};