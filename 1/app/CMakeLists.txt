cmake_minimum_required(VERSION 3.14)
project(parallel_lab_1)

# Для тестирования Google требуется как минимум C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Для Windows: Запретить переопределение параметров 
# компилятора/компоновщика родительского проекта
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  lab1_tests
  tests/GpuSpecTest.cpp
  tests/CpuSpecTest.cpp
  tests/RamSpecTest.cpp
  tests/LanSpecTest.cpp
  tests/ClusterTest.cpp
)
target_link_libraries(
  lab1_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lab1_tests)