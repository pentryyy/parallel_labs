#include <type_traits>
#ifndef HAS_SET_THREAD_COUNT_H
#define HAS_SET_THREAD_COUNT_H

template <typename VectorT>
struct HasSetThreadCount {
private:
    template <typename T>
    static auto check(int) -> decltype(std::declval<T>().setThreadCount(std::declval<int>()), std::true_type{});

    // Заглушка для случая, когда метода нет
    template <typename>
    static std::false_type check(...);
public:
    using type = decltype(check<VectorT>(0));

    static constexpr bool value = type::value;
};

#endif