#include <iostream>
#include <stdexcept>
#include <cstring> // Для string и memcpy
#include <fstream>
#include <iomanip> // Для std::setw
#ifndef MATRIX_H
#define MATRIX_H

template <typename T>
class Matrix {
protected:
    std::size_t M = 0;
    std::size_t N = 0;
    T* Data = nullptr;
public:
    using Type = T;
 
    Matrix(std::size_t M, std::size_t N) {
        if (N == 0 || M == 0) {
            throw std::invalid_argument("Число строк и столбцов должно быть больше нуля.");
        }
        this->M = M;
        this->N = N;
        Data = new T[M * N]();
    }

    Matrix(const Matrix& Matrix) {
        M = Matrix.M;
        N = Matrix.N;
        Data = new T[M * N];
        std::memcpy(Data, Matrix.Data, M * N * sizeof(T));
    }

    ~Matrix() {}

    T& operator()(std::size_t i, std::size_t j) {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return this->Data[i * this->N + j];
    }

    T operator()(std::size_t i, std::size_t j) const {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return this->Data[i * this->N + j];
    }
};

#endif