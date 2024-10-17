#include <vector>
#include <numeric>
#include <stdexcept>
#include "Matrix.h"
#include "DiagonalMatrix.h"
#include "DenseMatrix.h"

template <typename MatrixT>
class BlockMatrix : public Matrix<typename MatrixT::Type> {
private:
    std::vector<std::vector<MatrixT>> MatrixBlocks;
    std::pair<std::size_t, std::size_t> MatrixMesh;
    std::pair<std::size_t, std::size_t> MatrixSize;
public:
    BlockMatrix(std::size_t M, std::size_t N) : Matrix<typename MatrixT::Type>(M, N) {}

    BlockMatrix(const std::vector<std::vector<MatrixT>>& MatrixBlocks, 
                const std::pair<std::size_t, std::size_t>& MatrixMesh,
                const std::pair<std::size_t, std::size_t>& MatrixSize)
        : Matrix<typename MatrixT::Type>(MatrixMesh.first * MatrixSize.first, MatrixMesh.second * MatrixSize.second),
          MatrixBlocks(MatrixBlocks), MatrixMesh(MatrixMesh), MatrixSize(MatrixSize) {
        
        if (MatrixSize.first == 0 || MatrixSize.second == 0) {
            throw std::invalid_argument("Число строк и столбцов в блоке должно быть больше нуля.");
        }
        for (std::size_t i = 0; i < (MatrixMesh.first * MatrixSize.first); ++i) {
            for (std::size_t j = 0; j < (MatrixMesh.second * MatrixSize.second); ++j) {
                std::size_t blockRow = i / MatrixSize.first;
                std::size_t blockCol = j / MatrixSize.second;
                std::size_t localRow = i % MatrixSize.first;
                std::size_t localCol = j % MatrixSize.second;
                (*this)(i, j) = MatrixBlocks[blockRow][blockCol](localRow, localCol);
            }
        }
    }

    ~BlockMatrix() {}

    BlockMatrix<MatrixT> operator+(const BlockMatrix<MatrixT>& other) const {
        if ((this->MatrixMesh.first * this->MatrixSize.first) != 
            (other.MatrixMesh.first * other.MatrixSize.first) || 
            (this->MatrixMesh.second * this->MatrixSize.second) != 
            (other.MatrixMesh.second * other.MatrixSize.second)) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }
        BlockMatrix<MatrixT> result(this->MatrixMesh.first * this->MatrixSize.first,
                              this->MatrixMesh.second * this->MatrixSize.second);
        for (std::size_t i = 0; i < (this->MatrixMesh.first * this->MatrixSize.first); ++i) {
            for (std::size_t j = 0; j < (this->MatrixMesh.second * this->MatrixSize.second); ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    BlockMatrix<MatrixT> operator-(const BlockMatrix<MatrixT>& other) const {
        if ((this->MatrixMesh.first * this->MatrixSize.first) != 
            (other.MatrixMesh.first * other.MatrixSize.first) || 
            (this->MatrixMesh.second * this->MatrixSize.second) != 
            (other.MatrixMesh.second * other.MatrixSize.second)) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }
        BlockMatrix<MatrixT> result(this->MatrixMesh.first * this->MatrixSize.first,
                              this->MatrixMesh.second * this->MatrixSize.second);
        for (std::size_t i = 0; i < (this->MatrixMesh.first * this->MatrixSize.first); ++i) {
            for (std::size_t j = 0; j < (this->MatrixMesh.second * this->MatrixSize.second); ++j) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return result;
    }

    BlockMatrix<MatrixT> operator*(const BlockMatrix<MatrixT>& other) const {
        if ((this->MatrixMesh.first * this->MatrixSize.first) != 
            (other.MatrixMesh.first * other.MatrixSize.first) || 
            (this->MatrixMesh.second * this->MatrixSize.second) != 
            (other.MatrixMesh.second * other.MatrixSize.second)) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }
        BlockMatrix<MatrixT> result(this->MatrixMesh.first * this->MatrixSize.first,
                              this->MatrixMesh.second * this->MatrixSize.second);
        for (std::size_t i = 0; i < (this->MatrixMesh.first * this->MatrixSize.first); ++i) {
            for (std::size_t j = 0; j < (this->MatrixMesh.second * this->MatrixSize.second); ++j) {
                result(i, j) = (*this)(i, j) * other(i, j);
            }
        }
        return result;
    }

   BlockMatrix<MatrixT> Transpose() const {
        std::pair<std::size_t, std::size_t> newMatrixMesh = {MatrixMesh.second, MatrixMesh.first};
        std::vector<std::vector<MatrixT>> transposedBlocks(newMatrixMesh.first, 
                                                                  std::vector<MatrixT>(newMatrixMesh.second,
                                                                                              MatrixT(MatrixSize.second, 
                                                                                                             MatrixSize.first)));
        for (std::size_t i = 0; i < MatrixMesh.first; ++i) {
            for (std::size_t j = 0; j < MatrixMesh.second; ++j) {
                transposedBlocks[j][i] = this->MatrixBlocks[i][j].Transpose();
            }
        }
        return BlockMatrix<MatrixT>(transposedBlocks, newMatrixMesh, MatrixSize);
    }

    BlockMatrix<MatrixT> ScalarMultiplication(typename MatrixT::Type scalar) const {
        BlockMatrix<MatrixT> result(this->MatrixMesh.first * this->MatrixSize.first,
                              this->MatrixMesh.second * this->MatrixSize.second);
        for (std::size_t i = 0; i < (this->MatrixMesh.first * this->MatrixSize.first); ++i) {
            for (std::size_t j = 0; j < (this->MatrixMesh.second * this->MatrixSize.second); ++j) {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }
        return result;
    }

    std::string GetClassHeader() const override {
        return "BlockMatrix";
    }

    static BlockMatrix<MatrixT> Import(const std::string& fileName,
                                 const std::pair<std::size_t, std::size_t>& MatrixMesh,
                                 const std::pair<std::size_t, std::size_t>& MatrixSize) {    
        std::size_t BlocksCount = MatrixMesh.first * MatrixMesh.second;
        std::size_t MatrixSizeСomposition = MatrixSize.first * MatrixSize.second;
        // Двумерный вектор для хранения блоков матриц
        std::vector<std::vector<MatrixT>> MatrixBlocks(MatrixMesh.first, 
                                                              std::vector<MatrixT>(MatrixMesh.second, 
                                                                                          MatrixT(MatrixSize.first,
                                                                                                         MatrixSize.second)));
        std::ifstream file("import/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла: " + fileName);
        }
        std::string header;
        std::getline(file, header);
        std::size_t rows, cols;
        file >> rows >> cols;
        BlockMatrix<MatrixT> matrixTemp(rows, cols);
        if (header != matrixTemp.GetClassHeader()) { 
            throw std::runtime_error("Неверный формат файла. Ожидается заголовок '" + matrixTemp.GetClassHeader() + "'.");
        }
        for (std::size_t i = 0; i < MatrixMesh.first; ++i) {
            for (std::size_t j = 0; j < MatrixMesh.second; ++j) {
                for (std::size_t row = 0; row < MatrixSize.first; ++row) {
                    for (std::size_t col = 0; col < MatrixSize.second; ++col) {
                        if (!(file >> MatrixBlocks[i][j](row, col))) {
                            throw std::runtime_error("Ошибка чтения данных из файла.");
                        }
                    }
                }
            }
        }
        file.close();
        BlockMatrix<MatrixT> matrix(MatrixBlocks, MatrixMesh, MatrixSize);    
        std::cout << "Импорт из файла успешно выполнен: " << fileName << std::endl;
        return matrix;
    }

    void Export(const std::string& fileName) const {
        Matrix<typename MatrixT::Type>::template Export<BlockMatrix<MatrixT>>(fileName);
    }
};