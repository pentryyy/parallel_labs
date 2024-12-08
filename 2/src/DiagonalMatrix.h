#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <algorithm> 
#include "XMLMatrices/XMLDiagonalMatrixParser.h"

template <typename T>
class DiagonalMatrix : public XMLDiagonalMatrixParser<T> {
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

    // Метод для получения количества строк
    std::size_t rows() const {
        return this->matrixSize;
    }

    // Метод для получения количества столбцов
    std::size_t cols() const {
        return this->matrixSize;
    }

    // Получение значений для диагонали с заданным смещением
    std::vector<T> getDiagonal(int diagonalIndex) const {
        auto it = this->mapOfValuesForDiagonals.find(diagonalIndex);
        if (it != this->mapOfValuesForDiagonals.end()) {
            return it->second;
        }
        return std::vector<T>(this->matrixSize - std::abs(diagonalIndex), T{});
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

        return result;
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

        return result;
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

        return result;
    }

    // Транспонирование матрицы
    DiagonalMatrix<T> Transpose() const {
        DiagonalMatrix<T> transposed(this->matrixSize);

        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            transposed.mapOfValuesForDiagonals[-diagonalIndex] = values;
        }

        return transposed;
    }

    // Умножение на скаляр
    DiagonalMatrix<T> ScalarMultiplication(T scalar) const {
        DiagonalMatrix<T> result(this->matrixSize);

        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            result.mapOfValuesForDiagonals[diagonalIndex] = values;
            for (size_t i = 0; i < values.size(); ++i) {
                result.mapOfValuesForDiagonals[diagonalIndex][i] *= scalar;
            }
        }

        return result;
    }

    // Вывод матрицы
    void print(int width = 10) const {
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