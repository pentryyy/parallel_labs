#ifndef IMATRIX_ADDITIONAL_ACTIONS_H
#define IMATRIX_ADDITIONAL_ACTIONS_H

template <typename T>
class IMatrixAdditionalActions {
public:
    virtual ~IMatrixAdditionalActions() = default;

    // Вывод матрицы
    virtual void print(int width) const = 0;

    // Получение числа столбцов
    virtual std::size_t cols() const = 0;

    // Получение числа строк
    virtual std::size_t rows() const = 0;
};

#endif