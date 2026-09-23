#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <map>
#include <list>
#include <vector>
#include "Order.h"
#include "Trade.h"

class OrderBook {
private:
    std::map<double, std::list<Order>> bids;
    std::map<double, std::list<Order>> asks;

    std::map<long long, Order> orderLookup;

public:
    void addOrder(const Order& order);
    std::vector<Trade> matchOrder(Order order);
    bool cancelOrder(long long orderId);
    bool reduceOrder(long long orderId, int quantity);
    bool modifyOrder(long long orderId, double newPrice, int newQuantity);
    void printBook() const;
};

#endif