#include <iomanip>
#include "XMLMatrices/XMLDenseMatrixParser.h"

template <typename T>
class DenseMatrix : public XMLDenseMatrixParser<T> {
public:
    using Type = T; // Для получения используемого типа данных в матрице

    DenseMatrix() : XMLDenseMatrixParser<T>() {}

    DenseMatrix(std::size_t M, std::size_t N) : XMLDenseMatrixParser<T>(M, N) {}

    ~DenseMatrix() {}

    // Обращение к данным матрицы, например matrix(0, 0)
    T operator()(std::size_t i, std::size_t j) const {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return this->Data[i * this->N + j];
    }

    // Установка значения для элемента матрицы, например matrix(0, 1) = 5
    T& operator()(std::size_t i, std::size_t j) {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return this->Data[i * this->N + j];
    }
    
    // Метод для получения количества строк
    std::size_t rows() const {
        return this->M;
    }

    // Метод для получения количества столбцов
    std::size_t cols() const {
        return this->N;
    }

    DenseMatrix<T> operator+(const DenseMatrix<T>& other) const {
        if (this->M != other.M || this->N != other.N) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }
        DenseMatrix<T> result(this->M, this->N);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    DenseMatrix<T> operator-(const DenseMatrix<T>& other) const {
        if (this->M != other.M || this->N != other.N) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }
        DenseMatrix<T> result(this->M, this->N);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return result;
    }

    DenseMatrix<T> operator*(const DenseMatrix<T>& other) const {
        if (this->M != other.M || this->N != other.N) {
            throw std::invalid_argument("Количество столбцов первой матрицы должно быть равно количеству строк второй матрицы.");
        }
        DenseMatrix<T> result(this->M, this->N);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < other.N; ++j) {
                result(i, j) = 0;
                for (std::size_t k = 0; k < this->N; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    DenseMatrix<T> Transpose() const {
        DenseMatrix<T> result(this->N, this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    DenseMatrix<T> ScalarMultiplication(T scalar) const {
        DenseMatrix<T> result(this->M, this->N); 
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                result(i, j) = (*this)(i, j) * scalar; 
            }
        }
        return result;
    }

    // Вывод матрицы
    void print(int width = 10) const {
        std::cout << "Результат вывода плотной матрицы (" << this->M << " x " << this->N << ")\n";
        for (std::size_t i = 0; i < this->M; ++i) {
            std::cout << "| ";
            for (std::size_t j = 0; j < this->N; ++j) {
                std::cout << std::setw(width) << (*this)(i, j) << " ";
            }
            std::cout << "|\n";
        }
    }
};