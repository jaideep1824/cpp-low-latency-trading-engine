#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../include/Order.h"
#include "../include/OrderBook.h"

int main() {
    std::ifstream file("message_10.csv");

    if (!file.is_open()) {
        std::cout << "Could not open file.\n";
        return 1;
    }

    OrderBook book;

    std::string line;
    int count = 0;

    while (std::getline(file, line) && count < 1000) {

        std::stringstream ss(line);

        double time;
        int eventType;
        long long orderId;
        int quantity;
        int price;
        int direction;

        char comma;

        ss >> time >> comma
           >> eventType >> comma
           >> orderId >> comma
           >> quantity >> comma
           >> price >> comma
           >> direction;

        if (eventType == 1) {

            Order order;

            order.id = orderId;
            order.quantity = quantity;
            order.price = price / 10000.0;

            if (direction == 1) {
                order.side = OrderSide::BUY;
            } else {
                order.side = OrderSide::SELL;
            }

            book.addOrder(order);

        } else if (eventType == 2) {

            book.reduceOrder(orderId, quantity);

        } else if (eventType == 3) {

            book.cancelOrder(orderId);
        }

        count++;
    }

    file.close();

    std::cout << "\n===== ORDER BOOK FROM REAL DATA =====\n";

    book.printBook();

    return 0;
}