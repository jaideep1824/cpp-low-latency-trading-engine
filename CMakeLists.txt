cmake_minimum_required(VERSION 3.15)

project(LowLatencyTradingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(orderbook_demo
    examples/main.cpp
    src/OrderBook.cpp
)

add_executable(orderbook_tests
    tests/OrderBookTest.cpp
    src/OrderBook.cpp
)

add_executable(orderbook_benchmark
    benchmarks/OrderBookBenchmark.cpp
    src/OrderBook.cpp
)