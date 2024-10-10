#include "Matrix.h"

template <typename T>
class DenseMatrix : public Matrix<T> {
public:
    DenseMatrix(std::size_t M, std::size_t N) : Matrix<T>(M, N) {}

    ~DenseMatrix() {}
    
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

    DenseMatrix<T>* Transpose() const override {
        DenseMatrix<T>* result = new DenseMatrix<T>(this->N, this->M);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                (*result)(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    DenseMatrix<T>* ScalarMultiplication(T scalar) const override {
        DenseMatrix<T>* result = new DenseMatrix<T>(this->M, this->N);
        for (std::size_t i = 0; i < this->M; ++i) {
            for (std::size_t j = 0; j < this->N; ++j) {
                (*result)(i, j) = (*this)(i, j) * scalar;
            }
        }
        return result;
    }

    static DenseMatrix<T> Import(const std::string& fileName) {
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла: " + fileName);
        }
        std::string header;
        std::getline(file, header);
        if (header != "DenseMatrix") {
            throw std::runtime_error("Неверный формат файла. Ожидается заголовок 'DenseMatrix'.");
        }
        std::size_t rows, cols;
        file >> rows >> cols;
        DenseMatrix<T> matrix(rows, cols);
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

    void Export(const std::string& fileName) const {
        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для записи: " + fileName);
        }
        file << "DenseMatrix" << std::endl;
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