#include <random>
#include <cmath>
#include "Vector.h"
#include <chrono>  // Для замера времени
#include <functional> // Для приема функции в качестве аргумента
#include "IOperations.h"

template <typename T>
class VectorSingleThread : public Vector<T>, public IOperations<T> {
public:
    VectorSingleThread(int n) : Vector<T>(n) {}

    ~VectorSingleThread() {}

    void initializedConst(T value) override {
        for (auto& elem : this->data) {
            elem = value;
        }
        this->isInitialized = true;
    }

    void initializedRandom(int startRandomValue, int endRandomValue) override {
        std::random_device randomDevice;
        std::mt19937 randomGenerator(randomDevice());
        std::uniform_int_distribution<std::mt19937::result_type> randomRange(startRandomValue, endRandomValue);

        for (auto& elem : this->data) {
            elem = randomRange(randomGenerator);
        }
        this->isInitialized = true;
    }

    T minimumValue() override {
        this->checkInitialization();
        
        T minValue = this->data[0];
        for (const T& elem : this->data) {
            if (elem < minValue) {
                minValue = elem;
            }
        }
        return minValue;
    }

    size_t minimumIndexByValue() override {
        this->checkInitialization();
        
        T minValue = this->data[0];
        size_t minIndex = 0;
        
        for (size_t i = 1; i < this->data.size(); ++i) {
            if (this->data[i] < minValue) {
                minValue = this->data[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    T maximumValue() override {
        this->checkInitialization();
        
        T maxValue = this->data[0];
        for (const T& elem : this->data) {
            if (elem > maxValue) {
                maxValue = elem;
            }
        }
        return maxValue;
    }

    size_t maximumIndexByValue() override {
        this->checkInitialization();
        
        T maxValue = this->data[0];
        size_t maxIndex = 0;
        
        for (size_t i = 1; i < this->data.size(); ++i) {
            if (this->data[i] > maxValue) {
                maxValue = this->data[i];
                maxIndex = i;
            }
        }
        return maxIndex;
    }

    T avgValue() override {
        this->checkInitialization();
        return static_cast<T>(sumValue()) / this->data.size();
    }

    unsigned __int32 sumValue() override {
        this->checkInitialization();

        T sum = T(0);
        for (const T& elem : this->data) {
            sum += elem;
        }
        return sum;
    }

    unsigned __int64 euclidMonheton() override {
        this->checkInitialization();

        T sumOfSquares = T(0);
        for (const T& elem : this->data) {
            sumOfSquares += elem * elem;
        }
        return std::sqrt(static_cast<T>(sumOfSquares));
    }

    unsigned __int64 scalarMultiply(const Vector<T>& other) override {
        this->checkInitialization();
        
        if (this->data.size() != other.getData().size()) {
            throw std::invalid_argument("Размеры векторов не совпадают");
        }
        
        T productSum = T(0);
        for (size_t i = 0; i < this->data.size(); ++i) {
            productSum += this->data[i] * other.getData()[i];
        }
        return productSum;
    }

    void createTestData(const std::string& fileName) override {
        this->checkInitialization();

        std::ofstream file("export/" + fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла для записи: " + fileName);
        }

        auto measureTime = [this, &file](const std::function<void()>& func, const std::string& funcName) {
            // Для коректтной записи значений в файл
            static bool firstCall = true;
            if (!firstCall) { file << '\n'; } 
            else { firstCall = false; }
            
            file << funcName;

            auto startTime = std::chrono::high_resolution_clock::now();
            
            func();  // Выполняем переданную функцию

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endTime - startTime;
            
            file << '\n' << elapsed.count();
        };

        measureTime([this]() { minimumValue(); }, "minimumValue");
        measureTime([this]() { minimumIndexByValue(); }, "minimumIndexByValue");
        measureTime([this]() { maximumValue(); }, "maximumValue");
        measureTime([this]() { maximumIndexByValue(); }, "maximumIndexByValue");
        measureTime([this]() { sumValue(); }, "sumValue");
        measureTime([this]() { avgValue(); }, "avgValue"); 
        measureTime([this]() { euclidMonheton(); }, "euclidMonheton");
        measureTime([this]() { scalarMultiply(*this); }, "scalarMultiply");

        file.close();
        std::cout << "Данные тестирования успешно созданы: " << fileName << '\n';
    }
};