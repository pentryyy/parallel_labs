#include <iostream>
#include <string>

class CpuSpec {
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

    void Import(std::istream& in) {
        std::getline(in, model);
        in >> pcoreCount
           >> ecoreCount
           >> hasEcore
           >> hasMultithreading
           >> pcoreFrequency
           >> ecoreFrequency
           >> uncoreFrequency;
        in.ignore();
    }

    void Export(std::ostream& out) const {
        out << model << std::endl
            << pcoreCount << std::endl
            << ecoreCount << std::endl
            << hasEcore << std::endl
            << hasMultithreading << std::endl
            << pcoreFrequency << std::endl
            << ecoreFrequency << std::endl
            << uncoreFrequency;
    }

    bool Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        std::string header;
        std::getline(file, header);
        if (header != "CpuSpec") {
            std::cerr << "Неверный формат файла. Ожидается заголовок 'CpuSpec'." << std::endl;
            return false;
        }
        std::getline(file, model);
        file >> pcoreCount
             >> ecoreCount
             >> hasEcore
             >> hasMultithreading
             >> pcoreFrequency
             >> ecoreFrequency
             >> uncoreFrequency;
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
        file << "CpuSpec" << std::endl
            << model << std::endl
            << pcoreCount << std::endl
            << ecoreCount << std::endl
            << hasEcore << std::endl
            << hasMultithreading << std::endl
            << pcoreFrequency << std::endl
            << ecoreFrequency << std::endl
            << uncoreFrequency;
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};