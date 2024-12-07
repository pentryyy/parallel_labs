#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include "../../../rapidxml/rapidxml.hpp"

template <typename T>
class XMLDenseMatrixParser { 
protected:
    std::size_t M    = 0;       // Количество строк
    std::size_t N    = 0;       // Количество столбцов
    T*          Data = nullptr; // Данные матрицы

    XMLDenseMatrixParser(std::size_t M, std::size_t N) : M(M), N(N) {
        if (N == 0 || M == 0) {
            throw std::invalid_argument("Число строк и столбцов должно быть больше нуля");
        }
        Data = new T[M * N]();
    }

    XMLDenseMatrixParser() {}

    ~XMLDenseMatrixParser() {
        delete[] Data;
    }
public:
    void importFromXML(const std::string& filename) {
        std::ifstream file("import/" + filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть " + filename);
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
            throw std::runtime_error("Недопустимая структура XML: отсутствует корневой элемент <import>");
        }

        // Читаем размеры матрицы
        auto* numberOfLinesNode = root->first_node("numberOfLines");
        if (!numberOfLinesNode) {
            throw std::runtime_error("Отсутствует элемент <numberOfLines>");
        }
        M = std::stoul(numberOfLinesNode->value());

        auto* numberOfColumnsNode = root->first_node("numberOfColumns");
        if (!numberOfColumnsNode) {
            throw std::runtime_error("Отсутствует элемент <numberOfColumns>");
        }
        N = std::stoul(numberOfColumnsNode->value());

        // Выделяем память и заполняем массив нулями
        Data = new T[M * N];
        std::memset(Data, 0, M * N * sizeof(T));

        // Заполняем матрицу
        for (auto* node = root->first_node(); node; node = node->next_sibling()) {
            std::string nodeName = node->name();
            
            if (nodeName.find("valuesOfIndex") == 0) {
                size_t index = std::stoul(nodeName.substr(13)); 

                // Пропуск строк с индексом, превышающим M
                if (index + 1 >  M) {
                    continue; 
                }

                // Считываем строку значений
                std::string values = node->value();
                std::vector<T> rowValues;
                std::string temp;
                for (char ch : values) {
                    if (ch == ',') {
                        rowValues.push_back(static_cast<T>(std::stod(temp)));
                        temp.clear();
                    } else if (!std::isspace(ch)) {
                        temp += ch;
                    }
                }

                if (!temp.empty()) {
                    rowValues.push_back(static_cast<T>(std::stod(temp)));
                }

                // Если строка данных больше, чем необходимо, обрезаем лишние элементы
                if (rowValues.size() > N) {
                    rowValues.resize(N);
                }

                // Если строка данных меньше, чем необходимо, добавляем нули
                while (rowValues.size() < N) {
                    rowValues.push_back(T(0));
                }

                // Копируем значения строки в массив Data по индексу
                for (std::size_t col = 0; col < N; ++col) {
                    Data[index * N + col] = rowValues[col];
                }
            }
        }
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << filename << '\n';
    }

    void exportToXML(const std::string& filename) {
        std::ofstream file("export/" + filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }

        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<import>\n";
        
        // Запись размера матрицы
        file << "  <numberOfLines>" << M << "</numberOfLines>\n";
        file << "  <numberOfColumns>" << N << "</numberOfColumns>\n";
        
        // Запись значений для каждой строки
        for (std::size_t row = 0; row < M; ++row) {

            std::stringstream valuesStream;
            for (std::size_t col = 0; col < N; ++col) {
                valuesStream << Data[row * N + col];
                if (col != N - 1) {
                    valuesStream << ", ";
                }
            }

            // Формирование имени узла для текущей строки
            std::string nodeName = "valuesOfIndex" + std::to_string(row);
            file << "  <" << nodeName << ">" << valuesStream.str() << "</" << nodeName << ">\n";
        }

        file << "</import>";
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << filename << '\n';
    }
};