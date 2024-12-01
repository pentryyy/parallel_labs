#include <random>
#include <cmath>
#include <list>
#include <omp.h>
#include "Vector.h"
#include "IOperations.h"

template <typename T>
class VectorOpenMP : public Vector<T>, public IOperations<T> {
public:
    VectorOpenMP(size_t n) : Vector<T>(n) {}

    ~VectorOpenMP() {}

    void setThreadCount(int count) { omp_set_num_threads(count); }

    void initializedConst(T value) override {
        if (value == 0) {
            throw std::runtime_error("Деление на ноль запрещено");
        }

        #pragma omp parallel for
        for (size_t i = 0; i < this->data.size(); ++i) {
            this->data[i] = value;
        }
        this->isInitialized = true;
    }

    void initializedRandom(int startRandomValue, int endRandomValue) override {
        if (startRandomValue == 0 || endRandomValue == 0) {
            throw std::runtime_error("Деление на ноль запрещено");
        }

        if (!(startRandomValue < endRandomValue)) {
            throw std::invalid_argument("Начальное значение должно быть меньше конечного");
        }

        std::random_device randomDevice;
        std::mt19937 randomGenerator(randomDevice());
        std::uniform_int_distribution<std::mt19937::result_type> randomRange(startRandomValue, endRandomValue);

        #pragma omp parallel
        {
            std::mt19937 threadGenerator(randomDevice() + omp_get_thread_num());
            #pragma omp for
            for (size_t i = 0; i < this->data.size(); ++i) {
                this->data[i] = randomRange(threadGenerator);
            }
        }
        this->isInitialized = true;
    }

    void invertValues() override {
        this->checkInitialization();

        #pragma omp parallel for
        for (size_t i = 0; i < this->data.size(); ++i) {
            this->data[i] = static_cast<T>(1) / this->data[i];
        }
    }

    T minimumValue() override {
        this->checkInitialization();

        T minValue = this->data[0];
        #pragma omp parallel for reduction(min:minValue)
        for (size_t i = 0; i < this->data.size(); ++i) {
            if (this->data[i] < minValue) {
                minValue = this->data[i];
            }
        }
        return minValue;
    }

    size_t minimumIndexByValue() override {
        this->checkInitialization();

        std::list<T> listOfIndexes;
        T minValue = this->data[0];
       
        #pragma omp parallel
        {
            T localMin = minValue;
            size_t localIndex = 0;

            #pragma omp for nowait
            for (size_t i = 0; i < this->data.size(); ++i) {
                if (this->data[i] < localMin) {
                    localMin = this->data[i];
                    localIndex = i;
                }
            }

            #pragma omp critical
            {
                if (localMin < minValue) { listOfIndexes.push_back(localIndex); }
                listOfIndexes.sort();
            }
        }
        return listOfIndexes.front();
    }

    T maximumValue() override {
        this->checkInitialization();

        T maxValue = this->data[0];
        #pragma omp parallel for reduction(max:maxValue)
        for (size_t i = 0; i < this->data.size(); ++i) {
            if (this->data[i] > maxValue) {
                maxValue = this->data[i];
            }
        }
        return maxValue;
    }

    size_t maximumIndexByValue() override {
        this->checkInitialization();

        std::list<T> listOfIndexes;
        T maxValue = this->data[0];

        #pragma omp parallel
        {
            T localMax = maxValue;
            size_t localIndex = 0;

            #pragma omp for nowait
            for (size_t i = 0; i < this->data.size(); ++i) {
                if (this->data[i] > localMax) {
                    localMax = this->data[i];
                    localIndex = i;
                }
            }

            #pragma omp critical
            {
                if (localMax > maxValue) { listOfIndexes.push_back(localIndex); }
                listOfIndexes.sort();
            }
        }
        return listOfIndexes.front();
    }

    T avgValue() override {
        this->checkInitialization();
        return static_cast<T>(sumValue()) / this->data.size();
    }

    unsigned __int32 sumValue() override {
        this->checkInitialization();

        T sum = T(0);
        #pragma omp parallel for reduction(+:sum)
        for (size_t i = 0; i < this->data.size(); ++i) {
            sum += this->data[i];
        }
        return sum;
    }

    unsigned __int64 euclidMonheton() override {
        this->checkInitialization();

        T sumOfSquares = T(0);
        #pragma omp parallel for reduction(+:sumOfSquares)
        for (size_t i = 0; i < this->data.size(); ++i) {
            sumOfSquares += this->data[i] * this->data[i];
        }
        return std::sqrt(sumOfSquares);
    }

    unsigned __int32 scalarMultiply(const Vector<T>& other) override {
        this->checkInitialization();

        if (this->data.size() != other.getData().size()) {
            throw std::invalid_argument("Размеры векторов не совпадают");
        }

        T productSum = T(0);
        #pragma omp parallel for reduction(+:productSum)
        for (size_t i = 0; i < this->data.size(); ++i) {
            productSum += this->data[i] * other.getData()[i];
        }
        return productSum;
    }
};