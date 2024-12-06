#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <algorithm> 
#include "XMLDiagonalMatrixParser.h"

template <typename T>
class DiagonalMatrixAlt : public XMLDiagonalMatrixParser<T> {
public:
    DiagonalMatrixAlt(size_t size) : XMLDiagonalMatrixParser<T>(size) {}

    DiagonalMatrixAlt() : XMLDiagonalMatrixParser<T>() {}

    ~DiagonalMatrixAlt() {}

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

    // Установка значения для произвольной диагонали
    void set(size_t i, size_t j, T value) {
        if (i >= this->matrixSize || j >= this->matrixSize) {
            throw std::out_of_range("Индексы вне диапазона матрицы.");
        }
        
        // Вычисляем индекс, задем значение позиции в mapOfValuesForDiagonals 
        int diagonalIndex = static_cast<int>(j) - static_cast<int>(i);
        size_t pos = diagonalIndex >= 0 ? i : j;
    
        // Изменяем размерность матрицы на ту, что в импорте - |diagonalIndex|
        this->mapOfValuesForDiagonals[diagonalIndex].resize(this->matrixSize - std::abs(diagonalIndex), T{});
        this->mapOfValuesForDiagonals[diagonalIndex][pos] = value;
    }

    // Получение значений для диагонали с заданным смещением
    std::vector<T> getDiagonal(int diagonalIndex) const {
        auto it = this->mapOfValuesForDiagonals.find(diagonalIndex);
        if (it != this->mapOfValuesForDiagonals.end()) {
            return it->second;
        }
        return std::vector<T>(this->matrixSize - std::abs(diagonalIndex), T{});
    }

    // Метод для оптимизации данных (удаление лишних значений в диагоналях)
    void dataOptimization() {
        for (auto it = this->mapOfValuesForDiagonals.begin(); it != this->mapOfValuesForDiagonals.end();) {
            auto& [diagonalIndex, values] = *it;
            size_t newSize = this->matrixSize - std::abs(diagonalIndex);

            if (values.size() > newSize) {
                values.resize(newSize);
            }

            // Если вектор пуст, удаляем пару (ключ, значения) из mapOfValuesForDiagonals
            if (std::all_of(values.begin(), values.end(), [](const T& value) { return value == T{}; })) {
                it = this->mapOfValuesForDiagonals.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Оператор сложения
    DiagonalMatrixAlt<T> operator+(const DiagonalMatrixAlt<T>& other) const {
        if (this->matrixSize != other.matrixSize) {
            throw std::invalid_argument("Размеры матриц должны совпадать.");
        }

        DiagonalMatrixAlt<T> result(this->matrixSize);
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
    DiagonalMatrixAlt<T> operator-(const DiagonalMatrixAlt<T>& other) const {
        if (this->matrixSize != other.matrixSize) {
            throw std::invalid_argument("Размеры матриц должны совпадать.");
        }

        DiagonalMatrixAlt<T> result(this->matrixSize);
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
    DiagonalMatrixAlt<T> operator*(const DiagonalMatrixAlt<T>& other) const {
        if (this->matrixSize != other.matrixSize) {
            throw std::invalid_argument("Размеры матриц должны совпадать.");
        }

        DiagonalMatrixAlt<T> result(this->matrixSize);
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
    DiagonalMatrixAlt<T> Transpose() const {
        DiagonalMatrixAlt<T> transposed(this->matrixSize);

        for (const auto& [diagonalIndex, values] : this->mapOfValuesForDiagonals) {
            transposed.mapOfValuesForDiagonals[-diagonalIndex] = values;
        }

        return transposed;
    }

    // Умножение на скаляр
    DiagonalMatrixAlt<T> ScalarMultiplication(T scalar) const {
        DiagonalMatrixAlt<T> result(this->matrixSize);

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
        std::cout << "Результат вывода диагональной матрицы \n";
        for (std::size_t i = 0; i < this->matrixSize; ++i) {
            std::cout << "| ";
            for (std::size_t j = 0; j < this->matrixSize; ++j) {
                std::cout << std::setw(width) << (*this)(i, j) << " ";
            }
            std::cout << "|\n";
        }
    }
};