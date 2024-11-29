#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>

class TestData {
private:
    std::string                 funcName;
    std::list<std::list<float>> allFuncTimeValues;
public:
    TestData(const std::string& funcName, const std::list<std::list<float>>& allFuncTimeValues) :
        funcName(funcName),
        allFuncTimeValues(allFuncTimeValues) {}

    std::string getFuncName() const {
        return funcName;
    }

    size_t getSizeOfList() const {
        return allFuncTimeValues.size();
    }

    /*
    Через итератор ищем номер результата, 
    с заданным количеством потоков
    */
    float findMin(size_t threadNumber) const {
        auto it = allFuncTimeValues.begin();
        std::advance(it, threadNumber - 1);
        const auto& funcTimeValues = *it;

        return *std::min_element(funcTimeValues.begin(), funcTimeValues.end());
    }

    float findMax(size_t threadNumber) const {
        auto it = allFuncTimeValues.begin();
        std::advance(it, threadNumber - 1);
        const auto& funcTimeValues = *it;

        return *std::max_element(funcTimeValues.begin(), funcTimeValues.end());
    }

    float calculateMean(size_t threadNumber) const {
        auto it = allFuncTimeValues.begin();
        std::advance(it, threadNumber - 1);
        const auto& funcTimeValues = *it;

        float sum = std::accumulate(funcTimeValues.begin(), funcTimeValues.end(), 0.0f);
        return sum / funcTimeValues.size();
    }

    float calculateMedian(size_t threadNumber) const {
        auto it = allFuncTimeValues.begin();
        std::advance(it, threadNumber - 1);
        const auto& funcTimeValues = *it;

        std::vector<float> sortedValues(funcTimeValues.begin(), funcTimeValues.end());
        std::sort(sortedValues.begin(), sortedValues.end());

        size_t size = sortedValues.size();
        if (size % 2 == 0) {
            return (sortedValues[size / 2 - 1] + sortedValues[size / 2]) / 2.0f;
        } else {
            return sortedValues[size / 2];
        }
    }

    float calculatePercentile95(size_t threadNumber) const {
        auto it = allFuncTimeValues.begin();
        std::advance(it, threadNumber - 1);
        const auto& funcTimeValues = *it;

        std::vector<float> sortedValues(funcTimeValues.begin(), funcTimeValues.end());
        std::sort(sortedValues.begin(), sortedValues.end());

        size_t index = static_cast<size_t>(std::ceil(0.95 * sortedValues.size())) - 1;
        return sortedValues[index];
    }
};