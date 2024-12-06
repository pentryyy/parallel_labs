#include <iostream>
#include <string>
#include <fstream>
#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

class InputOutput {
public:
    virtual std::string GetClassHeader() const = 0;
    virtual bool Import(std::istream& in) = 0; 
    virtual void Export(std::ostream& out) const = 0; 
    virtual ~InputOutput() {} 

    /* 
    Метод, который проверяет соответствие названия заголовка и названия класса.
    Метод получает файл и если строка заголовка соответсвует
    названию класса, возвращает истину.
    */
    bool CheckHeaderName(std::istream& in) {
        std::string header;
        std::getline(in, header);
        if (header != GetClassHeader()) {
            std::cerr << "Неверный формат файла. Ожидается заголовок '" << GetClassHeader() << "'." << std::endl;
            return false;
        }
        return true;
    }

    bool isImportFromFile(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
            return false;
        }
        if (!Import(file)) { return false; }
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << fileName << std::endl;
        return true;
    }

    bool ExportToFile(const std::string& fileName) const {
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи: " << fileName << std::endl;
            return false;
        }
        Export(file);
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
        return true;
    }
};

#endif