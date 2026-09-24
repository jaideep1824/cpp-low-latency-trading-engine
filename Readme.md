# C++ Low-Latency Trading Engine

A simplified in-memory electronic trading engine built in C++17.

The project implements an order book and matching engine using price-time priority, supporting order submission, matching, partial fills, cancellation, quantity reduction, and modification.

## Features

* In-memory limit order book
* Separate bid and ask sides
* Price-time priority
* Full and partial order matching
* Order cancellation
* Order quantity reduction
* Order modification
* Order ID lookup
* Trade generation
* Manual unit tests
* Performance benchmark
* Historical market-data replay experiment using LOBSTER data

## Architecture

```text
Incoming Order
      |
      v
+-------------------+
|   Matching Engine |
+-------------------+
      |
      v
+-------------------+
|    Order Book     |
|                   |
| Bids       Asks   |
+-------------------+
      |
      v
    Trades
```

The order book maintains price levels using:

```text
Bids: std::map<double, std::list<Order>>
Asks: std::map<double, std::list<Order>>
```

Orders at the same price are stored in insertion order, allowing FIFO processing at each price level.

An additional order ID lookup structure is used to make cancellation and quantity reduction more direct.

## Matching Logic

The engine follows price-time priority.

### Buy order

A buy order can match the lowest available ask when:

```text
buy price >= ask price
```

### Sell order

A sell order can match the highest available bid when:

```text
sell price <= bid price
```

Within the same price level, the oldest order is matched first.

The engine supports both complete and partial fills.

## Supported Operations

### Add Order

Adds a new order to the appropriate side of the book.

### Match Order

Attempts to immediately execute an incoming order against compatible resting orders.

### Cancel Order

Removes an existing order using its order ID.

### Reduce Order

Reduces the quantity of an existing order, which is also used when processing execution/cancellation events from market data.

### Modify Order

Changes the price and quantity of an existing order.

A price modification removes the old order and re-enters it at the new price.

## Project Structure

```text
cpp-low-latency-trading-engine/
│
├── include/
│   ├── Order.h
│   ├── Trade.h
│   └── OrderBook.h
│
├── src/
│   └── OrderBook.cpp
│
├── tests/
│   └── OrderBookTest.cpp
│
├── benchmarks/
│   └── OrderBookBenchmark.cpp
│
├── examples/
│   ├── main.cpp
│   └── ReadMarketData.cpp
│
├── message_10.csv
└── CMakeLists.txt
```

## Testing

The project includes manual tests covering:

* Order insertion
* Full matching
* Partial matching
* Price priority
* Order cancellation
* Order modification

The tests are implemented in:

```text
tests/OrderBookTest.cpp
```

## Performance

The matching engine was benchmarked using synthetic workloads.

|    Orders | Average time/order |       Throughput |
| --------: | -----------------: | ---------------: |
|    10,000 |           0.184 µs | 5.44M orders/sec |
|   100,000 |           0.208 µs | 4.81M orders/sec |
| 1,000,000 |           0.241 µs | 4.15M orders/sec |

The 1-million-order workload processed approximately **4.15 million orders/sec** on the development machine.

These are synthetic single-process benchmark results and are intended to measure the current implementation rather than represent production exchange performance.

## Historical Market Data

The project also includes an experiment replaying historical limit-order-book message data.

The data contains events such as new order submissions and order cancellations/reductions. The replay demonstrates how real market messages can be parsed and used to populate an in-memory order book.

This experiment is intentionally kept separate from the core matching-engine benchmark.

## Build

Using g++:

```bash
g++ -std=c++17 examples/main.cpp src/OrderBook.cpp -o orderbook_demo
```

Run:

```bash
./orderbook_demo
```

Build the tests:

```bash
g++ -std=c++17 tests/OrderBookTest.cpp src/OrderBook.cpp -o orderbook_tests
```

Run:

```bash
./orderbook_tests
```

Build the benchmark:

```bash
g++ -std=c++17 benchmarks/OrderBookBenchmark.cpp src/OrderBook.cpp -o benchmark
```

Run:

```bash
./benchmark
```

## Design Considerations

The current implementation prioritizes clarity and correctness of the matching logic.

The use of `std::map` and `std::list` provides straightforward price-level and FIFO behavior, but a production low-latency trading system could use more specialized data structures and memory-management techniques.

Potential future improvements include:

* Cache-friendly order storage
* More efficient order cancellation
* Custom memory pools
* Avoiding unnecessary allocations
* Integer tick prices instead of floating-point prices
* Lock-free or low-contention components where appropriate
* More detailed latency measurements
* More comprehensive market-data replay

## Technologies

* C++17
* STL
* CMake
* Git
* Historical LOBSTER market data
* g++ / MSYS2
