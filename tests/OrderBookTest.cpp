#include <iostream>
#include <vector>

#include "../include/Order.h"
#include "../include/OrderBook.h"
#include "../include/Trade.h"

void printResult(const char* testName, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
    }
}

void testAddOrder() {
    OrderBook book;

    Order order;
    order.id = 1;
    order.side = OrderSide::BUY;
    order.price = 100.00;
    order.quantity = 50;

    book.addOrder(order);

    printResult("Add order", true);
}

void testFullMatch() {
    OrderBook book;

    Order buy;
    buy.id = 1;
    buy.side = OrderSide::BUY;
    buy.price = 100.00;
    buy.quantity = 50;

    book.addOrder(buy);

    Order sell;
    sell.id = 2;
    sell.side = OrderSide::SELL;
    sell.price = 100.00;
    sell.quantity = 50;

    std::vector<Trade> trades = book.matchOrder(sell);

    bool passed = false;

    if (trades.size() == 1 &&
        trades[0].buyOrderId == 1 &&
        trades[0].sellOrderId == 2 &&
        trades[0].quantity == 50) {

        passed = true;
    }

    printResult("Full match", passed);
}

void testPartialMatch() {
    OrderBook book;

    Order buy;
    buy.id = 1;
    buy.side = OrderSide::BUY;
    buy.price = 100.00;
    buy.quantity = 100;

    book.addOrder(buy);

    Order sell;
    sell.id = 2;
    sell.side = OrderSide::SELL;
    sell.price = 100.00;
    sell.quantity = 40;

    std::vector<Trade> trades = book.matchOrder(sell);

    bool passed = false;

    if (trades.size() == 1 &&
        trades[0].quantity == 40) {

        passed = true;
    }

    printResult("Partial match", passed);
}

void testPricePriority() {
    OrderBook book;

    Order buy1;
    buy1.id = 1;
    buy1.side = OrderSide::BUY;
    buy1.price = 99.00;
    buy1.quantity = 50;

    Order buy2;
    buy2.id = 2;
    buy2.side = OrderSide::BUY;
    buy2.price = 100.00;
    buy2.quantity = 50;

    book.addOrder(buy1);
    book.addOrder(buy2);

    Order sell;
    sell.id = 3;
    sell.side = OrderSide::SELL;
    sell.price = 99.00;
    sell.quantity = 30;

    std::vector<Trade> trades = book.matchOrder(sell);

    bool passed = false;

    if (trades.size() == 1 &&
        trades[0].buyOrderId == 2 &&
        trades[0].quantity == 30) {

        passed = true;
    }

    printResult("Price priority", passed);
}

void testCancelOrder() {
    OrderBook book;

    Order buy;
    buy.id = 1;
    buy.side = OrderSide::BUY;
    buy.price = 100.00;
    buy.quantity = 50;

    book.addOrder(buy);

    bool cancelled = book.cancelOrder(1);

    printResult("Cancel order", cancelled);
}

void testModifyOrder() {
    OrderBook book;

    Order buy;
    buy.id = 1;
    buy.side = OrderSide::BUY;
    buy.price = 100.00;
    buy.quantity = 50;

    book.addOrder(buy);

    bool modified = book.modifyOrder(1, 101.00, 75);

    printResult("Modify order", modified);
}

int main() {
    std::cout << "===== ORDER BOOK TESTS =====\n\n";

    testAddOrder();
    testFullMatch();
    testPartialMatch();
    testPricePriority();
    testCancelOrder();
    testModifyOrder();

    return 0;
}