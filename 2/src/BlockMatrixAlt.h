#include <vector>
#include <stdexcept>
#include <memory>
#include <iomanip>
#include <iostream>
#include "IMatrixAdditionalActions.h"

template <typename MatrixType>
class BlockMatrixAlt : public IMatrixAdditionalActions<MatrixType> {
private:
    std::size_t                                           blockRows, blockCols;
    std::vector<std::vector<std::shared_ptr<MatrixType>>> blocks;
public:
    using Type = typename MatrixType::Type;

    // Конструктор с указанием числа блоков
    BlockMatrixAlt(std::size_t blockRows, std::size_t blockCols) : 
                   blockRows(blockRows), blockCols(blockCols) {
        blocks.resize(blockRows, std::vector<std::shared_ptr<MatrixType>>(blockCols, nullptr));
    }

    ~BlockMatrixAlt() {}

    // Обращение к данным матрицы, например matrix(0, 0)
    Type operator()(std::size_t i, std::size_t j) const {
        std::size_t blockRow = i / blocks[0][0]->rows();
        std::size_t blockCol = j / blocks[0][0]->cols();

        if (blockRow >= blockRows || blockCol >= blockCols) {
            throw std::out_of_range("Индексы выходят за пределы блочной матрицы.");
        }

        std::size_t localRow = i % blocks[0][0]->rows();
        std::size_t localCol = j % blocks[0][0]->cols();

        if (localRow >= blocks[0][0]->rows() || localCol >= blocks[0][0]->cols()) {
            throw std::out_of_range("Индексы выходят за пределы блока.");
        }

        // Пустой блок возвращает 0
        if (!blocks[blockRow][blockCol]) {
            return static_cast<Type>(0);
        }

        return (*blocks[blockRow][blockCol])(localRow, localCol);
    }

    // Установка значения для элемента матрицы, например matrix(0, 1) = make_shared <another matrix>
    std::shared_ptr<MatrixType>& operator()(std::size_t row, std::size_t col) {
        if (row >= blockRows || col >= blockCols) {
            throw std::out_of_range("Индексы блока выходят за пределы блочной матрицы.");
        }
        return blocks[row][col];
    }

    std::size_t rows() const override {
        return blockRows * blocks[0][0]->rows();
    }

    std::size_t cols() const override {
        return blockCols * blocks[0][0]->cols();
    }

    void print(int width = 10) const override {
        for (std::size_t i = 0; i < rows(); ++i) {
            std::cout << "| ";
            for (std::size_t j = 0; j < cols(); ++j) {
                std::cout << std::setw(width) << (*this)(i, j) << " ";
            }
            std::cout << "|\n";
        }
    }
};
