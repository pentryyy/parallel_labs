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

    virtual T& operator()(std::size_t i, std::size_t j) = 0;
    virtual T operator()(std::size_t i, std::size_t j) const = 0;
    virtual std::string GetClassHeader()  const = 0;

    void Print() const {
        std::cout << "Результат вывода матрицы " << this->M << " x " << this->N << std::endl;
        for (std::size_t i = 0; i < M; ++i) {
            std::cout << "| ";
            for (std::size_t j = 0; j < N; ++j) {
                std::cout << std::setw(10) << (*this)(i, j) << " ";
            }
            std::cout << "|" << std::endl;
        }
    }

    template<typename MatrixT>
    static MatrixT Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла: " + fileName);
        }
        std::string header;
        std::getline(file, header);
        std::size_t rows, cols;
        file >> rows >> cols;
        MatrixT matrix(rows, cols);
        if (header != matrix.GetClassHeader()) { 
            throw std::runtime_error("Неверный формат файла. Ожидается заголовок '" + matrix.GetClassHeader() + "'.");
        }
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                if (!(file >> (matrix)(i, j))) {
                    throw std::runtime_error("Ошибка чтения данных из файла.");
                }
            }
        } 
        file.close();
        std::cout << "Импорт из файла успешно выполнен: " << fileName << std::endl;
        return matrix;
    }

    template<typename MatrixT>
    void Export(const std::string& fileName) const {
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для записи: " + fileName);
        }
        file << this->GetClassHeader() << std::endl;
        file << this->M << " " << this->N;
        for (std::size_t i = 0; i < this->M; ++i) {
            file << std::endl;
            for (std::size_t j = 0; j < this->N; ++j) {
                file << (*this)(i, j) << " ";
            }
        }
        file.close();
        std::cout << "Экспорт в файл успешно выполнен: " << fileName << std::endl;
    }
};

#endif