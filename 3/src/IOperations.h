#ifndef IOPERATIONS_H
#define IOPERATIONS_H

template<typename T>
class IOperations {
public:
    virtual void initializedConst(T value) = 0;
    virtual void initializedRandom(int startRandomValue, int endRandomValue) = 0;

    virtual T minimumValue() = 0;
    virtual size_t minimumIndexByValue() = 0;

    virtual T maximumValue() = 0;
    virtual size_t maximumIndexByValue() = 0;

    virtual T avgValue() = 0;
    virtual unsigned __int32 sumValue() = 0;
    virtual unsigned __int64 euclidMonheton() = 0;
    virtual unsigned __int64 scalarMultiply(const Vector<T>& other) = 0;
};

#endif