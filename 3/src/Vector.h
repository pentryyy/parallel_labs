#include <vector>
#include <fstream>
#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector {
protected:
    std::vector<T> data;
    bool isInitialized = false;

    void checkInitialization() const {
        if (!isInitialized) {
            throw std::logic_error("Объект должен быть инициализирован");
        }
    }
public:
    Vector(int n) {
        std::vector<T> newData(n);
        this->data = newData;
    }

    ~Vector() {}

    const std::vector<T>& getData() const {
        return data;
    }

    void exportToFile(const std::string& fileName) {
        checkInitialization();

        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для записи: " + fileName);
        }
        file << "number of elements: " << data.size() << '\n';
        for (const T& value : data) {
            file << value << '\n';
        }
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << '\n';
    }
    
    void importFromFile(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для чтения: " + fileName);
        }
        
        std::string numberOfValuesLine;
        std::getline(file, numberOfValuesLine);

        auto extractNumberOfValues = [&numberOfValuesLine]() -> size_t {
            std::string substringToRemove = "number of elements: ";
            size_t pos = numberOfValuesLine.find(substringToRemove);
            if (pos != std::string::npos) {
                numberOfValuesLine.erase(pos, substringToRemove.length());
            }
            return std::stoul(numberOfValuesLine);
        };

        T value;
        size_t numberOfValues = extractNumberOfValues();

        if (data.size() != numberOfValues){
            throw std::out_of_range("Количество значений в файле не соответствует заданному в объекте");
        }

        data.clear();
        while (file >> value) {
            if (value == 0){
                throw std::runtime_error("Деление на ноль запрещено");
            }
            
            data.push_back(value);
        }

        this->isInitialized = true;
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << fileName << std::endl;
    }
};

#endif