#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <cmath>
#include <random>
#include <chrono>  // Для замера времени
#include <functional> // Для приема функции в качестве аргумента
#include "Vector.h"
#include "IOperations.h"

template <typename T>
class VectorMultiThread : public Vector<T>, public IOperations<T> {
private:
    int threadCount = 1; // Значение по умолчанию - 1 поток
    std::mutex mutex;

    template <typename Tfunc>
    void runInParallel(Tfunc func) {
        std::vector<std::thread> threads;
        size_t partSize = this->data.size() / threadCount;

        for (int i = 0; i < threadCount; ++i) {
            size_t start = i * partSize;
            size_t end = (i == threadCount - 1) ? this->data.size() : start + partSize;
            threads.emplace_back(func, start, end);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
public:
    VectorMultiThread(int n) : Vector<T>(n) {}

    ~VectorMultiThread() {}

    void setThreadCount(int count){ threadCount = count; }

    void initializedConst(T value) override {
        auto initTask = [this, value](size_t start, size_t end) {
            for (size_t i = start; i < end; ++i) {
                this->data[i] = value;
            }
        };

        runInParallel(initTask);
        this->isInitialized = true;
    }

    void initializedRandom(int startRandomValue, int endRandomValue) override {
        std::random_device randomDevice;
        std::mt19937 randomGenerator(randomDevice());
        std::uniform_int_distribution<std::mt19937::result_type> randomRange(startRandomValue, endRandomValue);

        auto initTask = [this, &randomGenerator, &randomRange](size_t start, size_t end) {
            for (size_t i = start; i < end; ++i) {
                this->data[i] = randomRange(randomGenerator);
            }
        };

        runInParallel(initTask);
        this->isInitialized = true;
    }

    T minimumValue() override {
        this->checkInitialization();

        T minValue = this->data[0];
        auto minTask = [this, &minValue](size_t start, size_t end) {
            T localMin = this->data[start];
            for (size_t i = start; i < end; ++i) {
                if (this->data[i] < localMin) {
                    localMin = this->data[i];
                }
            }
            std::lock_guard<std::mutex> lock(mutex);
            if (localMin < minValue) minValue = localMin;
        };

        runInParallel(minTask);
        return minValue;
    }

    size_t minimumIndexByValue() override {
        this->checkInitialization();

        std::list<T> listOfIndexes;
        T minValue = this->data[0];

        auto minIndexTask = [this, &minValue, &listOfIndexes](size_t start, size_t end) {
            T localMin = this->data[start];
            size_t localIndex = start;
            for (size_t i = start; i < end; ++i) {
                if (this->data[i] < localMin) {
                    localMin = this->data[i];
                    localIndex = i;
                }
            }
            std::lock_guard<std::mutex> lock(mutex);
            if (localMin < minValue) { listOfIndexes.push_back(localIndex); }
        };

        runInParallel(minIndexTask);
        listOfIndexes.sort();
        return listOfIndexes.front(); // Первое вхождение минимального элемента
    }

    T maximumValue() override {
        this->checkInitialization();

        T maxValue = this->data[0];
        auto maxTask = [this, &maxValue](size_t start, size_t end) {
            T localMax = this->data[start];
            for (size_t i = start; i < end; ++i) {
                if (this->data[i] > localMax) {
                    localMax = this->data[i];
                }
            }
            std::lock_guard<std::mutex> lock(mutex);
            if (localMax > maxValue) maxValue = localMax;
        };

        runInParallel(maxTask);
        return maxValue;
    }

    size_t maximumIndexByValue() override {
        this->checkInitialization();

        std::list<T> listOfIndexes;
        T maxValue = this->data[0];
       
        auto maxIndexTask = [this, &maxValue, &listOfIndexes](size_t start, size_t end) {
            T localMax = this->data[start];
            size_t localIndex = start;
            for (size_t i = start; i < end; ++i) {
                if (this->data[i] > localMax) {
                    localMax = this->data[i];
                    localIndex = i;
                }
            }
            std::lock_guard<std::mutex> lock(mutex);
            if (localMax > maxValue) { listOfIndexes.push_back(localIndex); }
        };

        runInParallel(maxIndexTask);
        listOfIndexes.sort();
        return listOfIndexes.front(); // Первое вхождение максимального элемента
    }

    T avgValue() override {
        this->checkInitialization();
        
        return static_cast<T>(sumValue()) / this->data.size();
    }

    unsigned __int32 sumValue() override {
        this->checkInitialization();
        
        T totalSum = T(0);
        auto sumTask = [this, &totalSum](size_t start, size_t end) {
            T localSum = T(0);
            for (size_t i = start; i < end; ++i) {
                localSum += this->data[i];
            }
            std::lock_guard<std::mutex> lock(mutex);
            totalSum += localSum;
        };

        runInParallel(sumTask);
        return totalSum;
    }

    unsigned __int64 euclidMonheton() override {
        this->checkInitialization();

        T sumOfSquares = T(0);
        auto euclidTask = [this, &sumOfSquares](size_t start, size_t end) {
            T localSum = T(0);
            for (size_t i = start; i < end; ++i) {
                localSum += this->data[i] * this->data[i];
            }
            std::lock_guard<std::mutex> lock(mutex);
            sumOfSquares += localSum;
        };

        runInParallel(euclidTask);
        return std::sqrt(sumOfSquares);
    }

    unsigned __int64 scalarMultiply(const Vector<T>& other) override {
        this->checkInitialization();

        if (this->data.size() != other.getData().size()) {
            throw std::invalid_argument("Размеры векторов не совпадают");
        }

        T productSum = T(0);
        auto scalarTask = [this, &productSum, &other](size_t start, size_t end) {
            T localSum = T(0);
            for (size_t i = start; i < end; ++i) {
                localSum += this->data[i] * other.getData()[i];
            }
            std::lock_guard<std::mutex> lock(mutex);
            productSum += localSum;
        };

        runInParallel(scalarTask);
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

            // Количество итераций равно количеству потоков процессора
            for(int i = 1; i <= std::thread::hardware_concurrency(); ++i) {
                this->threadCount = i;
                auto startTime = std::chrono::high_resolution_clock::now();
                
                func();  // Выполняем переданную функцию

                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = endTime - startTime;
                
                file << '\n' << elapsed.count();
            }
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