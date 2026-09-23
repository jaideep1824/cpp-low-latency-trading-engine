#include <iostream>
#include <chrono>
#include <vector>

#include "../include/Order.h"
#include "../include/OrderBook.h"
#include "../include/Trade.h"

void runBenchmark(int numberOfOrders) {

    OrderBook book;

    for (int i = 0; i < numberOfOrders; ++i) {
        Order sell;

        sell.id = i;
        sell.side = OrderSide::SELL;
        sell.price = 100.00;
        sell.quantity = 10;

        book.addOrder(sell);
    }

    auto start = std::chrono::high_resolution_clock::now();

    long long totalTradedQuantity = 0;

    for (int i = 0; i < numberOfOrders; ++i) {

        Order buy;

        buy.id = numberOfOrders + i;
        buy.side = OrderSide::BUY;
        buy.price = 100.00;
        buy.quantity = 10;

        std::vector<Trade> trades = book.matchOrder(buy);

        for (std::vector<Trade>::iterator it = trades.begin();
             it != trades.end();
             ++it) {

            totalTradedQuantity += it->quantity;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> elapsed = end - start;

    double ordersPerSecond =
        numberOfOrders / (elapsed.count() / 1000000.0);

    std::cout << "\nOrders: " << numberOfOrders << "\n";
    std::cout << "Total traded quantity: "
              << totalTradedQuantity << "\n";

    std::cout << "Time: "
              << elapsed.count()
              << " microseconds\n";

    std::cout << "Average: "
              << elapsed.count() / numberOfOrders
              << " microseconds/order\n";

    std::cout << "Throughput: "
              << ordersPerSecond
              << " orders/sec\n";
}

int main() {

    std::cout << "===== ORDER BOOK BENCHMARK =====\n";

    runBenchmark(10000);
    runBenchmark(100000);
    runBenchmark(1000000);

    return 0;
}