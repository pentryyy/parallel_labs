#include <iostream>
#include <string>

class GpuSpec {
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
    
    void Print() const { 
        std::cout << "Модель GPU: " << model
                  << ", Объем памяти: " << memorySize << " МБ"
                  << ", Количество ядер: " << coreCount
                  << ", Частота ядер: " << coreFrequency << " МГц"
                  << ", Частота VRAM: " << vramFrequency << " МГц" << std::endl;
    }

    void Import(std::istream& in) {
        std::getline(in, model);
        in >> memorySize 
           >> coreCount 
           >> coreFrequency 
           >> vramFrequency;
        in.ignore();
    }

    void Export(std::ostream& out) const {
        out << model << std::endl
            << memorySize << std::endl
            << coreCount << std::endl
            << coreFrequency << std::endl
            << vramFrequency;
    }

    bool Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        std::string header;
        std::getline(file, header);
        if (header != "GpuSpec") {
            std::cerr << "Неверный формат файла. Ожидается заголовок 'GpuSpec'." << std::endl;
            return false;
        }
        std::getline(file, model);
        file >> memorySize
             >> coreCount
             >> coreFrequency
             >> vramFrequency;
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
        file << "GpuSpec" << std::endl
             << model << std::endl
             << memorySize << std::endl
             << coreCount << std::endl
             << coreFrequency << std::endl
             << vramFrequency;
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};