#include <vector>
#include <stdexcept>
#include <memory>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include "IMatrixAdditionalActions.h"

template <typename MatrixType>
class BlockMatrix : public IMatrixAdditionalActions<MatrixType> {
private:
    std::size_t                                           blockRows, blockCols;
    std::vector<std::vector<std::shared_ptr<MatrixType>>> blocks;
public:
    using Type = typename MatrixType::Type;

    // Конструктор с указанием числа блоков
    BlockMatrix(std::size_t blockRows, std::size_t blockCols) : 
                   blockRows(blockRows), blockCols(blockCols) {
        blocks.resize(blockRows, std::vector<std::shared_ptr<MatrixType>>(blockCols, nullptr));
    }

    ~BlockMatrix() {}

    // Обращение к данным матрицы, например matrix(0, 0)
    Type operator()(std::size_t i, std::size_t j) const {
        std::size_t blockRow = i / blocks[0][0]->rows();
        std::size_t blockCol = j / blocks[0][0]->cols();

        if (blockRow >= blockRows || blockCol >= blockCols) {
            throw std::out_of_range("Индексы выходят за пределы блочной матрицы.");
        }

        std::size_t localRow = i % blocks[0][0]->rows();
        std::size_t localCol = j % blocks[0][0]->cols();

        if (localRow >= blocks[0][0]->rows() || localCol >= blocks[0][0]->cols()) {
            throw std::out_of_range("Индексы выходят за пределы блока.");
        }

        // Пустой блок возвращает 0
        if (!blocks[blockRow][blockCol]) {
            return static_cast<Type>(0);
        }

        return (*blocks[blockRow][blockCol])(localRow, localCol);
    }

    // Установка значения для элемента матрицы, например matrix(0, 1) = make_shared <another matrix>
    std::shared_ptr<MatrixType>& operator()(std::size_t row, std::size_t col) {
        if (row >= blockRows || col >= blockCols) {
            throw std::out_of_range("Индексы блока выходят за пределы блочной матрицы.");
        }
        return blocks[row][col];
    }

    // Оператор сложения
    BlockMatrix<MatrixType> operator+(const BlockMatrix<MatrixType>& other) const {
        if (blockRows != other.blockRows || blockCols != other.blockCols) {
            throw std::invalid_argument("Размеры блоков должны совпадать.");
        }

        BlockMatrix<MatrixType> result(blockRows, blockCols);
        
        for (std::size_t i = 0; i < blockRows; ++i) {
            for (std::size_t j = 0; j < blockCols; ++j) {
                // Проверяем, что блоки не пустые в обеих матрицах
                if (blocks[i][j] && other.blocks[i][j]) {
                    result(i, j) = std::make_shared<MatrixType>(*blocks[i][j] + *other.blocks[i][j]);
                } else if (blocks[i][j]) {
                    // Если один из блоков пустой, просто копируем блок
                    result(i, j) = std::make_shared<MatrixType>(*blocks[i][j]);
                } else if (other.blocks[i][j]) {
                    // Если другой блок пустой, просто копируем блок
                    result(i, j) = std::make_shared<MatrixType>(*other.blocks[i][j]);
                }
            }
        }
        
        return result;
    }

    // Оператор вычитания
    BlockMatrix<MatrixType> operator-(const BlockMatrix<MatrixType>& other) const {
        if (blockRows != other.blockRows || blockCols != other.blockCols) {
            throw std::invalid_argument("Размеры блоков должны совпадать.");
        }

        BlockMatrix<MatrixType> result(blockRows, blockCols);
        for (std::size_t i = 0; i < blockRows; ++i) {
            for (std::size_t j = 0; j < blockCols; ++j) {
                // Проверяем, что оба блока не пусты
                if (blocks[i][j] && other.blocks[i][j]) {
                    result(i, j) = std::make_shared<MatrixType>(*blocks[i][j] - *other.blocks[i][j]);
                } else if (blocks[i][j]) {
                    // Если второй блок пуст, просто копируем первый блок
                    result(i, j) = std::make_shared<MatrixType>(*blocks[i][j]);
                } else if (other.blocks[i][j]) {
                    // Если первый блок пуст, просто копируем второй блок
                    result(i, j) = std::make_shared<MatrixType>(*other.blocks[i][j]);
                }
            }
        }
        return result;
    }

    // Оператор умножения
    BlockMatrix<MatrixType> operator*(const BlockMatrix<MatrixType>& other) const {
        if (blockRows != other.blockRows || blockCols != other.blockCols) {
            throw std::invalid_argument("Размеры блоков должны совпадать.");
        }
        
        BlockMatrix<MatrixType> result(blockRows, blockCols);
        for (std::size_t i = 0; i < blockRows; ++i) {
            for (std::size_t j = 0; j < blockCols; ++j) {
                if (blocks[i][j] && other.blocks[i][j]) {
                    result(i, j) = std::make_shared<MatrixType>(*blocks[i][j] * *other.blocks[i][j]);
                
                // Возвращаем пустые объекты класса в случае нахождения только одной матрицы
                } else if (blocks[i][j]) {
                    result(i, j) = std::make_shared<MatrixType>(blocks[i][j]->rows(),
                                                                blocks[i][j]->rows());
                } else if (other.blocks[i][j]) {
                    result(i, j) = std::make_shared<MatrixType>(other.blocks[i][j]->rows(),
                                                                other.blocks[i][j]->rows());
                }
            }
        }
        return result;
    }

    // Транспонирование матрицы
    BlockMatrix<MatrixType> transpose() const {
        BlockMatrix<MatrixType> result(blockCols, blockRows);
        for (std::size_t i = 0; i < blockRows; ++i) {
            for (std::size_t j = 0; j < blockCols; ++j) {

                // Транспонирование блока, если он существует
                if (blocks[i][j]) {
                    result(j, i) = std::make_shared<MatrixType>(blocks[i][j]->transpose());
                }
            }
        }
        return result;
    }

    // Скалярное произведение
    BlockMatrix<MatrixType> scalarMultiplication(Type scalar) const {
        BlockMatrix<MatrixType> result(blockRows, blockCols);
        for (std::size_t i = 0; i < blockRows; ++i) {
            for (std::size_t j = 0; j < blockCols; ++j) {

                // Выполняем умножение только для существующих блоков
                if (blocks[i][j]) {
                    result(j, i) = std::make_shared<MatrixType>(blocks[i][j]->scalarMultiplication(scalar));
                }
            }
        }
        return result;
    }

    std::size_t rows() const override {
        return blockRows * blocks[0][0]->rows();
    }

    std::size_t cols() const override {
        return blockCols * blocks[0][0]->cols();
    }

    void print(int width = 10) const override {
        for (std::size_t i = 0; i < rows(); ++i) {
            std::cout << "| ";
            for (std::size_t j = 0; j < cols(); ++j) {
                std::cout << std::setw(width) << (*this)(i, j) << " ";
            }
            std::cout << "|\n";
        }
    }

    void importFromDirectory(const std::string& directoryName, const std::string& directoryPath = "") {
        std::string importDirectoryPath = (directoryPath.empty() ? "import/" : directoryPath + "/" ) + directoryName;
        std::filesystem::path dirPath(importDirectoryPath);

        // Проверяем, существует ли директория
        if (!std::filesystem::exists(dirPath)) {
            throw std::runtime_error("Директория не существует: " + dirPath.string());
        }

        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".xml") {

                // Извлекаем имя файла без расширения и проверяем его корректность
                std::string filename = entry.path().filename().string();
                size_t underscorePos = filename.find('_');
                size_t dotPos = filename.find('.');

                if (underscorePos != std::string::npos && dotPos != std::string::npos) {

                    // Извлекаем индексы i, j и проверяем их корректность
                    int i = std::stoi(filename.substr(0, underscorePos));
                    int j = std::stoi(filename.substr(underscorePos + 1, dotPos - underscorePos - 1));

                    if (i >= 0 && i < blockRows && j >= 0 && j < blockCols) {

                        // Создаем объект DiagonalMatrix и загружаем данные из файла
                        blocks[i][j] = std::make_shared<MatrixType>();
                        blocks[i][j]->importFromXML(filename, importDirectoryPath);
                    } else {
                        throw std::out_of_range("Индексы вне диапазона: (" + std::to_string(i) + ", " + std::to_string(j) + ") в файле " + filename);
                    }
                }
            }
        }
        std::cout << "Импорт из директории успешно выполнен: " << directoryName << '\n';
    }

    void exportToDirectory(const std::string& directoryName, const std::string& directoryPath = "") {
        std::string exportDirectoryPath = (directoryPath.empty() ? "export/" : directoryPath + "/") + directoryName;
        std::filesystem::path dirPath(exportDirectoryPath);
        
        // Делаем экспорт только не пустых блоков
        for (std::size_t i = 0; i < blockRows; ++i) {
            for (std::size_t j = 0; j < blockCols; ++j) {
                if (blocks[i][j]) {
                    std::string blockName = std::to_string(i) + "_" + std::to_string(j) + ".xml";
                    blocks[i][j]->exportToXML(blockName, exportDirectoryPath);
                }
            }
        }
        std::cout << "Экспорт в директорию успешно выполнен: " << directoryName << '\n';
    }
};