#include "Matrix.h"

template <typename T>
class DenseMatrix : public Matrix<T> {
public:
    DenseMatrix(std::size_t M, std::size_t N) : Matrix<T>(M, N) {}

    // Оператор сложения
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

    // Оператор умножения
    DenseMatrix<T> operator*(const DenseMatrix<T>& other) const {
        if (this->N != other.M) {
            throw std::invalid_argument("Количество столбцов первой матрицы должно быть равно количеству строк второй матрицы.");
        }
        DenseMatrix<T> result(this->M, other.N);
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

    // Транспонирование матрицы
    Matrix<T>* transpose() const override {
        DenseMatrix<T>* result = new DenseMatrix<T>(this->N, this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                (*result)(j, i) = (*this)(i, j);
            }
        }
        return result;
    }
};
