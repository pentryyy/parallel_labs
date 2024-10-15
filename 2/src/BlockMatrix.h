#include <vector>
#include <numeric>
#include <stdexcept>
#include "Matrix.h"
#include "DenseMatrix.h"

template <typename T>
class BlockMatrix : public Matrix<T> {
private:
    std::vector<std::vector<DenseMatrix<T>>> blocks;
    std::vector<std::size_t> row_block_sizes;
    std::vector<std::size_t> col_block_sizes;

    // Метод расчета количества строк для конструктора матрицы
    std::size_t CalculateTotalRows(const std::vector<std::size_t>& sizes) {
        return std::accumulate(sizes.begin(), sizes.end(), std::size_t(0));
    }

    // Метод расчета количества столбцов для конструктора матрицы
    std::size_t CalculateTotalCols(const std::vector<std::size_t>& sizes) {
        return std::accumulate(sizes.begin(), sizes.end(), std::size_t(0));
    }

    std::pair<std::size_t, std::size_t> FindBlockIndex(std::size_t idx, const std::vector<std::size_t>& block_sizes) const {
        std::size_t accumulated_size = 0;
        for (std::size_t i = 0; i < block_sizes.size(); ++i) {
            if (idx < accumulated_size + block_sizes[i]) {
                return {i, idx - accumulated_size};
            }
            accumulated_size += block_sizes[i];
        }
        throw std::out_of_range("Индекс вне допустимых границ.");
    }
public:
    BlockMatrix(std::size_t numRows, 
                std::size_t numCols
            ) : Matrix<T>(numRows, numCols) {}
    BlockMatrix(const std::vector<std::vector<DenseMatrix<T>>>& blockMatrix, 
                const std::vector<std::size_t>& rowSizes, 
                const std::vector<std::size_t>& colSizes
            ) : Matrix<T>(CalculateTotalRows(rowSizes), CalculateTotalCols(colSizes)),
            blocks(blockMatrix), 
            row_block_sizes(rowSizes), 
            col_block_sizes(colSizes) {
        if (blockMatrix.size() != rowSizes.size() || blockMatrix[0].size() != colSizes.size()) {
            throw std::invalid_argument("Размеры блоков и подматриц не совпадают.");
        }
    }

    ~BlockMatrix() {}

    T& operator()(std::size_t i, std::size_t j) override {
        auto [block_row, row_in_block] = FindBlockIndex(i, row_block_sizes);
        auto [block_col, col_in_block] = FindBlockIndex(j, col_block_sizes);
        if (block_row >= blocks.size() || block_col >= blocks[0].size()) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return blocks[block_row][block_col](row_in_block, col_in_block);
    }

    T operator()(std::size_t i, std::size_t j) const override {
        auto [block_row, row_in_block] = FindBlockIndex(i, row_block_sizes);
        auto [block_col, col_in_block] = FindBlockIndex(j, col_block_sizes);
        if (block_row >= blocks.size() || block_col >= blocks[0].size()) {
            throw std::out_of_range("Индексы выходят за пределы матрицы.");
        }
        return blocks[block_row][block_col](row_in_block, col_in_block);
    }

    std::string GetClassHeader() const override {
        return "BlockMatrix";
    }

    void Export(const std::string& fileName) const {
        Matrix<T>::template Export<BlockMatrix<T>>(fileName);
    }

    static BlockMatrix<T> Import(const std::string& fileName) {
        BlockMatrix<T> matrix = Matrix<T>::template Import<BlockMatrix<T>>(fileName);
        return matrix;
    }   
};