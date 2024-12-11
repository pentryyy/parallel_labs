#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <algorithm> 
#include "XMLMatrices/XMLDiagonalMatrixParser.h"
#include "IMatrixAdditionalActions.h"

template <typename T>
class DiagonalMatrix : public XMLDiagonalMatrixParser<T>, public IMatrixAdditionalActions<T> {
public:
    using Type = T; // Для получения используемого типа данных в матрице

    // Для совместимости с блочной матрицей
    DiagonalMatrix(size_t M, size_t N) : XMLDiagonalMatrixParser<T>(M) {}

    DiagonalMatrix(size_t size) : XMLDiagonalMatrixParser<T>(size) {}

    DiagonalMatrix() : XMLDiagonalMatrixParser<T>() {}

    ~DiagonalMatrix() {}

    // Обращение к данным матрицы, например matrix(0, 0)
    T operator()(size_t i, size_t j) const {
        if (i >= this->matrixSize || j >= this->matrixSize) {
            throw std::out_of_range("Индексы вне диапазона матрицы.");
        }

        // Вычисляем индекс и ищем его в  mapOfValuesForDiagonals 
        int diagonalIndex = static_cast<int>(j) - static_cast<int>(i);

        auto it = this->mapOfValuesForDiagonals.find(diagonalIndex);
        if (it != this->mapOfValuesForDiagonals.end() && i < this->matrixSize && j < this->matrixSize) {
            size_t pos = diagonalIndex >= 0 ? i : j;
            if (pos < it->second.size()) {
                return it->second[pos];
            }
        }
        return T{};
    }

    // Установка значения для произвольной диагонали, например matrix(0, 1) = 5
    T& operator()(size_t i, size_t j) {
        if (i >= this->matrixSize || j >= this->matrixSize) {
            throw std::out_of_range("Индексы вне диапазона матрицы.");
        }
        
        // Вычисляем индекс диагонали
        int diagonalIndex = static_cast<int>(j) - static_cast<int>(i);
        size_t pos = diagonalIndex >= 0 ? i : j;
        
        // Убедимся, что вектор для данной диагонали имеет правильный размер
        this->mapOfValuesForDiagonals[diagonalIndex].resize(
            this->matrixSize - std::abs(diagonalIndex), T{}
        );

        // Возвращаем ссылку на значение
        return this->mapOfValuesForDiagonals[diagonalIndex][pos];
    }

    // Получение значений для диагонали с заданным смещением
    std::vector<T> getDiagonal(int diagonalIndex) const {
        auto it = this->mapOfValuesForDiagonals.find(diagonalIndex);
        if (it != this->mapOfValuesForDiagonals.end()) {
            return it->second;
        }
        return std::vector<T>(this->matrixSize - std::abs(diagonalIndex), T{});
    }

    // Удаление ключа диагонали из mapOfValuesForDiagonals
    void removeDiagonal(int diagonalIndex) {
        auto it = this->mapOfValuesForDiagonals.find(diagonalIndex);
        if (it != this->mapOfValuesForDiagonals.end()) {
            this->mapOfValuesForDiagonals.erase(it);
        }
    }

    // Удлаление ключа диагонали нулей из mapOfValuesForDiagonals
    DiagonalMatrix<T> checkZeros() {
        int rows = this->rows() - 1;
        for (int i = -1 * (rows); i <= rows; ++i) {
            bool allZero = true;
            for (const auto& item : this->getDiagonal(i)) {
                if (item != 0) {
                    allZero = false;
                    break;
                }
            }

            if (allZero) {
                this->removeDiagonal(i);
            }
        }
        return (*this);
    }

    // Оператор сложения
    DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& other) const {
        if (this->matrixSize != other.matrixSize) {
            throw std::invalid_argument("Размеры матриц должны совпадать.");
        }

        DiagonalMatrix<T> result(this->matrixSize);
        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            result.mapOfValuesForDiagonals[diagonalIndex] = values;
        }

        for (const auto& [diagonalIndex, values] : other.mapOfValuesForDiagonals) {
            for (size_t i = 0; i < values.size(); ++i) {
                result.mapOfValuesForDiagonals[diagonalIndex][i] += values[i];
            }
        }

        return result.checkZeros();
    }

    // Оператор вычитания
    DiagonalMatrix<T> operator-(const DiagonalMatrix<T>& other) const {
        if (this->matrixSize != other.matrixSize) {
            throw std::invalid_argument("Размеры матриц должны совпадать.");
        }

        DiagonalMatrix<T> result(this->matrixSize);
        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            result.mapOfValuesForDiagonals[diagonalIndex] = values;
        }

        for (const auto& [diagonalIndex, values] : other.mapOfValuesForDiagonals) {
            for (size_t i = 0; i < values.size(); ++i) {
                result.mapOfValuesForDiagonals[diagonalIndex][i] -= values[i];
            }
        }

        return result.checkZeros();
    }

    // Оператор умножения
    DiagonalMatrix<T> operator*(const DiagonalMatrix<T>& other) const {
        if (this->matrixSize != other.matrixSize) {
            throw std::invalid_argument("Размеры матриц должны совпадать.");
        }

        DiagonalMatrix<T> result(this->matrixSize);
        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            result.mapOfValuesForDiagonals[diagonalIndex] = values;
        }

        for (const auto& [diagonalIndex, values] : other.mapOfValuesForDiagonals) {
            for (size_t i = 0; i < values.size(); ++i) {
                result.mapOfValuesForDiagonals[diagonalIndex][i] *= values[i];
            }
        }

        return result.checkZeros();
    }

    // Транспонирование матрицы
    DiagonalMatrix<T> transpose() const {
        DiagonalMatrix<T> transposed(this->matrixSize);

        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            transposed.mapOfValuesForDiagonals[-diagonalIndex] = values;
        }

        return transposed.checkZeros();
    }

    // Умножение на скаляр
    DiagonalMatrix<T> scalarMultiplication(T scalar) const {
        DiagonalMatrix<T> result(this->matrixSize);

        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            result.mapOfValuesForDiagonals[diagonalIndex] = values;
            for (size_t i = 0; i < values.size(); ++i) {
                result.mapOfValuesForDiagonals[diagonalIndex][i] *= scalar;
            }
        }

        return result.checkZeros();
    }

    std::size_t rows() const override {
        return this->matrixSize;
    }

    std::size_t cols() const override {
        return this->matrixSize;
    }
    
    void print(int width = 10) const override {
        std::cout << "Результат вывода диагональной матрицы (" << this->matrixSize << ")\n";
        for (std::size_t i = 0; i < this->matrixSize; ++i) {
            std::cout << "| ";
            for (std::size_t j = 0; j < this->matrixSize; ++j) {
                std::cout << std::setw(width) << (*this)(i, j) << " ";
            }
            std::cout << "|\n";
        }
    }
};