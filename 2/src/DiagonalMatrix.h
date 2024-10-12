#include "Matrix.h"

template <typename T>
class DiagonalMatrix : public Matrix<T> {
public:
    DiagonalMatrix(std::size_t M) : Matrix<T>(M, M) {}
    DiagonalMatrix(std::size_t M, std::size_t N) : Matrix<T>(M, N) {
        if (M!=N){
            throw std::invalid_argument("Матрица должна быть квадратной.");
        }
    }

    ~DiagonalMatrix() {}
    
    static std::string GetClassHeader() {
        return "DiagonalMatrix";
    }

    void Export(const std::string& fileName) const {
        Matrix<T>::template Export<DiagonalMatrix<T>>(fileName);
    }

   static DiagonalMatrix<T> Import(const std::string& fileName) {
        DiagonalMatrix<T> matrix = Matrix<T>::template Import<DiagonalMatrix<T>>(fileName);
        return matrix;
    }

    DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& other) const {
        if (this->M != other.M){
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }
        DiagonalMatrix<T> result(this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = (*this)(i, i) + other(i, i);
        }
        return result;
    }

    DiagonalMatrix<T> operator-(const DiagonalMatrix<T>& other) const {
        if (this->M != other.M) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }
        DiagonalMatrix<T> result(this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = (*this)(i, i) - other(i, i);
        }
        return result;
    }

    DiagonalMatrix<T> operator*(const DiagonalMatrix<T>& other) const {
       if (this->M != other.M) {
            throw std::invalid_argument("Размеры матриц должны совпадать для умножения.");
        }
          DiagonalMatrix<T> result(this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = (*this)(i, i) * other(i, i);
        }
        return result;
    }

    // Транспонированная диагональная матрица такая же, как исходная
    DiagonalMatrix<T> Transpose() const {
        return *this;
    }

    DiagonalMatrix<T> ScalarMultiplication(T scalar) const {
        DiagonalMatrix<T> result(this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = this->Data[i] * scalar;
        }
        return result;
    }
};