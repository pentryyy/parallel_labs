#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cassert>

template <typename T>
class Matrix {
protected:
    std::size_t M = 0;
    std::size_t N = 0;
    T* Data = nullptr;
public:
    Matrix(std::size_t M, std::size_t N) {
        assert((N > 0) && (M > 0));
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

    auto operator()(std::size_t i, std::size_t j) const noexcept -> T {
        assert((i < N) && (j < M));
        return Data[i * N + j];
    }

    auto operator()(std::size_t i, std::size_t j) noexcept -> T& {
        assert((i < N) && (j < M));
        return Data[i * N + j];
    }

    virtual Matrix<T>* transpose() const = 0;
};