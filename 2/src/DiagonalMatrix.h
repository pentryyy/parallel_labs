#include "Matrix.h"
#include <vector>

template <typename T>
class DiagonalMatrix : public Matrix<T> {
private:
    std::vector<std::vector<T>> matrix;
public:
    // Нужен для того, чтобы импортировать матрицу из файла
    DiagonalMatrix(std::size_t M, std::size_t N) : Matrix<T>(M, N) {
        if (M != N) {
            throw std::invalid_argument("Матрица должна быть квадратной");
        }
    }

    DiagonalMatrix(const std::vector<T>& diagonal) : Matrix<T>(diagonal.size(), diagonal.size()) {
        std::size_t size = diagonal.size();
        for (std::size_t i = 0; i < size; ++i) {
            (*this)(i, i) = diagonal[i];
        }
    }
    
    ~DiagonalMatrix() {}

    T& operator()(std::size_t i, std::size_t j) {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        if (i != j) {
            static T zero = T(0); 
            return zero;
        }
        return this->Data[i * this->N + j];
    }

    T operator()(std::size_t i, std::size_t j) const {
        if (i >= this->M || j >= this->N) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        if (i != j) {
            return T(0); 
        }
        return this->Data[i * this->N + j];
    }

    DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& other) const {
        if (this->M != other.M) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }
        DiagonalMatrix<T> result(this->M, this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = (*this)(i, i) + other(i, i);
        }
        return result;
    }

    DiagonalMatrix<T> operator-(const DiagonalMatrix<T>& other) const {
        if (this->M != other.M) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }
        DiagonalMatrix<T> result(this->M, this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = (*this)(i, i) - other(i, i);
        }
        return result;
    }

    DiagonalMatrix<T> operator*(const DiagonalMatrix<T>& other) const {
        if (this->M != other.M) {
            throw std::invalid_argument("Размеры матриц должны совпадать для умножения.");
        }
        DiagonalMatrix<T> result(this->M, this->M);
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
        DiagonalMatrix<T> result(std::vector<T>(this->M, 0));
        for (std::size_t i = 0; i < this->M; ++i) {
            result(i, i) = (*this)(i, i) * scalar;
        }
        return result;
    }

    std::string GetClassHeader() const override {
        return "DiagonalMatrix";
    }

    void Export(const std::string& fileName) const {
        Matrix<T>::template Export<DiagonalMatrix<T>>(fileName);
    }

    static DiagonalMatrix<T> Import(const std::string& fileName) {
        DiagonalMatrix<T> matrix = Matrix<T>::template Import<DiagonalMatrix<T>>(fileName);
        return matrix;
    }
};