#include <iostream>
#include <stdexcept>
#include <cstring>
#include <fstream>
#ifndef MATRIX_H
#define MATRIX_H

template <typename T>
class Matrix {
protected:
    std::size_t M = 0;
    std::size_t N = 0;
    T* Data = nullptr;
public:
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

    ~Matrix() { delete[] Data; }

    auto operator()(std::size_t i, std::size_t j) const -> T {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return Data[i * N + j];
    }

    auto operator()(std::size_t i, std::size_t j) -> T& {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return Data[i * N + j];
    }

    void Print() const {
        std::cout << "Результат вывода матрицы "  << this->M <<" x " << this->N << std::endl
                  <<std::string(96, '-') << std::endl;
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                std::cout << Data[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::string(96, '-') << std::endl;
    }

    virtual Matrix<T>* Transpose() const = 0;
    virtual Matrix<T>* ScalarMultiplication(T scalar) const  = 0;
};

#endif