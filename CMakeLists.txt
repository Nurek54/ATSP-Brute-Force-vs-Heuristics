cmake_minimum_required(VERSION 3.20)
project(ATSP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ATSP_CORE
        src/core/TSPInstance.cpp
        src/core/TSPUtilities.cpp
        src/algorithms/BruteForceTSP.cpp
        src/algorithms/NearestNeighborTSP.cpp
        src/algorithms/RandomTSP.cpp
        src/io/SaveToCSV.cpp)

add_executable(atsp
        src/main.cpp
        src/app/Menu.cpp
        src/app/TSPSimulation.cpp
        ${ATSP_CORE})
target_include_directories(atsp PRIVATE src)
# Pozwala wpisac w menu sama nazwe pliku, np. matrix_6x6.atsp
target_compile_definitions(atsp PRIVATE ATSP_DATA_DIR="${CMAKE_SOURCE_DIR}/data")

# Pomiary do README: zawsze z optymalizacja, niezaleznie od typu builda
add_executable(benchmark tools/benchmark.cpp ${ATSP_CORE})
target_include_directories(benchmark PRIVATE src)
if (MSVC)
    target_compile_options(benchmark PRIVATE /O2)
else ()
    target_compile_options(benchmark PRIVATE -O2)
endif ()
