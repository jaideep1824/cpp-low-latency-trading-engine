#include <iostream>

#include "../include/Order.h"
#include "../include/OrderBook.h"

int main() {
    OrderBook book;

    Order buy1;
    buy1.id = 1;
    buy1.side = OrderSide::BUY;
    buy1.price = 100.00;
    buy1.quantity = 50;

    Order buy2;
    buy2.id = 2;
    buy2.side = OrderSide::BUY;
    buy2.price = 99.90;
    buy2.quantity = 100;

    Order sell1;
    sell1.id = 3;
    sell1.side = OrderSide::SELL;
    sell1.price = 100.10;
    sell1.quantity = 30;

    book.addOrder(buy1);
    book.addOrder(buy2);
    book.addOrder(sell1);

    std::cout << "INITIAL ORDER BOOK";
    book.printBook();

    std::cout << "\n\nModifying BUY order ID 1...\n";

    bool modified = book.modifyOrder(1, 101.00, 75);

    if (modified) {
        std::cout << "Order 1 modified successfully.\n";
    } else {
        std::cout << "Order 1 not found.\n";
    }

    std::cout << "\nFINAL ORDER BOOK";
    book.printBook();

    return 0;
}