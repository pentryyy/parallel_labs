#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include "../../../rapidxml/rapidxml.hpp"

template <typename T>
class XMLDiagonalMatrixParser {
protected:
    size_t                        matrixSize;
    std::map<int, std::vector<T>> mapOfValuesForDiagonals;

    XMLDiagonalMatrixParser(size_t size) : matrixSize(size) {
        if (size == 0) {
            throw std::invalid_argument("Размерность должна быть больше нуля");
        }
    }

    XMLDiagonalMatrixParser() {}

    ~XMLDiagonalMatrixParser() {
        this->mapOfValuesForDiagonals.clear();
    }
public:
    void importFromXML(const std::string& fileName, const std::string& filePath = "") {
        std::string fullPath = (filePath.empty() ? "import/" : filePath + "/") + fileName;

        std::ifstream file(fullPath);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть " + fileName);
        }

        // Считываем весь файл в строку
        std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
        buffer.push_back('\0');

        rapidxml::xml_document<> doc;
        doc.parse<0>(&buffer[0]);

        // Проверка структуры файла импорта
        rapidxml::xml_node<>* root = doc.first_node("import");
        if (!root) {
            throw std::runtime_error("Недопустимая структура XML: отсутствует корневой элемент");
        }

        // Извлекаем размер матрицы
        rapidxml::xml_node<>* sizeNode = root->first_node("matrixSize");
        if (!sizeNode) {
            throw std::runtime_error("Отсутствующий или недопустимый элемент <matrixSize>");
        }
        matrixSize = std::stoul(sizeNode->value());

        for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
           
            // Проверяем, что имя тега содержит "valuesOfIndex"
            if (std::string(node->name()).find("valuesOfIndex") != std::string::npos) {
                std::string nodeName = node->name();
                size_t indexPos = nodeName.find("valuesOfIndex");
                std::string indexStr = nodeName.substr(indexPos + 13);
                
                // Заменяем _ для отрицательных индексов и преобразуем в целое число
                if (indexStr[0] == '_') {
                    indexStr = "-" + indexStr.substr(1);
                }
                int index = std::stoi(indexStr);

                // Создаем вектор значений для индекса
                std::vector<T> values;
                std::stringstream ss(node->value());
                std::string item;
                while (std::getline(ss, item, ',')) {
                    std::stringstream itemStream(item);
                    T value;
                    itemStream >> value;
                    values.push_back(value);
                }

                size_t sizeOfDiagonals = matrixSize - std::abs(index);
                if (values.size() > sizeOfDiagonals) {
                    values.resize(sizeOfDiagonals);
                }

                mapOfValuesForDiagonals[index] = values;
            }
        }
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << fileName << '\n';
    }

    void exportToXML(const std::string& fileName, const std::string& filePath = "") {
        std::string fullPath = (filePath.empty() ? "export/" : filePath + "/") + fileName;

        std::ofstream file(fullPath);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + fileName);
        }

        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<import>\n";
        
        // Запись размера матрицы
        file << "  <matrixSize>" << matrixSize << "</matrixSize>\n";
        
        // Запись значений для каждой диагонали
        for (const auto& entry : mapOfValuesForDiagonals) {
            int index = entry.first;
            const std::vector<T>& values = entry.second;

            // Формирование строки значений для текущей диагонали
            std::stringstream valuesStream;
            for (size_t i = 0; i < values.size(); ++i) {
                valuesStream << values[i];
                if (i != values.size() - 1) {
                    valuesStream << ", ";
                }
            }

            std::string nodeName;
            if (index < 0) {
                nodeName = "valuesOfIndex_" + std::to_string(std::abs(index));
            } else {
                nodeName = "valuesOfIndex" + std::to_string(index);
            }

            // Запись значений диагонали в XML
            file << "  <" << nodeName << ">" << valuesStream.str() << "</" << nodeName << ">\n";
        }

        file << "</import>";
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << '\n';
    }
};