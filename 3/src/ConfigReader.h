#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../rapidxml/rapidxml.hpp"

class ConfigReader {
private:
    int         numberOfThreads;
    int         numberOfTests;
    size_t      vectorSize;
    std::string fileForTestTime;
    std::string fileForTestMetrics;
public:
    ConfigReader(const std::string& fileName) {
        std::ifstream file("config/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть " + fileName);
        }

        // Считываем весь файл в строку
        std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
        buffer.push_back('\0');

        rapidxml::xml_document<> doc;
        doc.parse<0>(&buffer[0]);

        rapidxml::xml_node<>* root = doc.first_node("config");
        if (!root) {
            throw std::runtime_error("Недопустимая структура XML: отсутствует корневой элемент");
        }

        // Читаем <numberOfThreads>
        rapidxml::xml_node<>* threadsElement = root->first_node("numberOfThreads");
        if (threadsElement && threadsElement->value()) {
            numberOfThreads = std::stoi(threadsElement->value());
        } else {
            throw std::runtime_error("Отсутствующий или недопустимый элемент <numberOfThreads>");
        }

        // Читаем <numberOfTests>
        rapidxml::xml_node<>* testsElement = root->first_node("numberOfTests");
        if (testsElement && testsElement->value()) {
            numberOfTests = std::stoi(testsElement->value());
        } else {
            throw std::runtime_error("Отсутствующий или недопустимый элемент <numberOfTests>");
        }

        // Читаем <vectorSize>
        rapidxml::xml_node<>* vectorElement = root->first_node("vectorSize");
        if (vectorElement && vectorElement->value()) {
            vectorSize = std::stoi(vectorElement->value());
        } else {
            throw std::runtime_error("Отсутствующий или недопустимый элемент <vectorSize>");
        }

        // Читаем <fileForTestTime>
        rapidxml::xml_node<>* timePathElement = root->first_node("fileForTestTime");
        if (timePathElement && timePathElement->value()) {
            fileForTestTime = timePathElement->value();
        } else {
            throw std::runtime_error("Отсутствующий или недопустимый элемент <fileForTestTime>");
        }

        // Читаем <fileForTestMetrics>
        rapidxml::xml_node<>* dataPathElement = root->first_node("fileForTestMetrics");
        if (dataPathElement && dataPathElement->value()) {
            fileForTestMetrics = dataPathElement->value();
        } else {
            throw std::runtime_error("Отсутствующий или недопустимый элемент <fileForTestMetrics>");
        }
    }

    int getNumberOfThreads() const {
        return numberOfThreads;
    }

    int getNumberOfTests() const {
        return numberOfTests;
    }

    size_t getVectorSize() const {
        return vectorSize;
    }

    std::string getFileForTestTime() const {
        return fileForTestTime;
    }

    std::string getfileForTestMetrics() const {
        return fileForTestMetrics;
    }
};