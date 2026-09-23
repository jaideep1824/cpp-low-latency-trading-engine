#include "../include/OrderBook.h"
#include "../include/Trade.h"
#include <iostream>
#include <vector>

void OrderBook::addOrder(const Order& order) {
    if (order.side == OrderSide::BUY) {
        bids[order.price].push_back(order);
    } else {
        asks[order.price].push_back(order);
    }

    orderLookup[order.id] = order;
}

std::vector<Trade> OrderBook::matchOrder(Order order) {
    std::vector<Trade> trades;

    if (order.side == OrderSide::BUY) {

        while (order.quantity > 0 && !asks.empty()) {

            std::map<double, std::list<Order>>::iterator askLevel;
            askLevel = asks.begin();

            if (order.price < askLevel->first) {
                break;
            }

            std::list<Order>& ordersAtPrice = askLevel->second;

            while (order.quantity > 0 && !ordersAtPrice.empty()) {

                Order& sellOrder = ordersAtPrice.front();

                int tradedQuantity;

                if (order.quantity < sellOrder.quantity) {
                    tradedQuantity = order.quantity;
                } else {
                    tradedQuantity = sellOrder.quantity;
                }

                Trade trade;

                trade.buyOrderId = order.id;
                trade.sellOrderId = sellOrder.id;
                trade.price = sellOrder.price;
                trade.quantity = tradedQuantity;

                trades.push_back(trade);

                order.quantity -= tradedQuantity;
                sellOrder.quantity -= tradedQuantity;

                if (sellOrder.quantity == 0) {
                    ordersAtPrice.pop_front();
                }
            }

            if (ordersAtPrice.empty()) {
                asks.erase(askLevel);
            }
        }

        if (order.quantity > 0) {
            bids[order.price].push_back(order);
        }

    } else {

        while (order.quantity > 0 && !bids.empty()) {

            std::map<double, std::list<Order>>::iterator bidLevel;
            bidLevel = bids.end();
            --bidLevel;

            if (order.price > bidLevel->first) {
                break;
            }

            std::list<Order>& ordersAtPrice = bidLevel->second;

            while (order.quantity > 0 && !ordersAtPrice.empty()) {

                Order& buyOrder = ordersAtPrice.front();

                int tradedQuantity;

                if (order.quantity < buyOrder.quantity) {
                    tradedQuantity = order.quantity;
                } else {
                    tradedQuantity = buyOrder.quantity;
                }

                Trade trade;

                trade.buyOrderId = buyOrder.id;
                trade.sellOrderId = order.id;
                trade.price = buyOrder.price;
                trade.quantity = tradedQuantity;

                trades.push_back(trade);

                order.quantity -= tradedQuantity;
                buyOrder.quantity -= tradedQuantity;

                if (buyOrder.quantity == 0) {
                    ordersAtPrice.pop_front();
                }
            }

            if (ordersAtPrice.empty()) {
                bids.erase(bidLevel);
            }
        }

        if (order.quantity > 0) {
            asks[order.price].push_back(order);
        }
    }

    return trades;
}

bool OrderBook::cancelOrder(long long orderId) {

    std::map<long long, Order>::iterator lookupIt = orderLookup.find(orderId);

    if (lookupIt == orderLookup.end()) {
        return false;
    }

    Order order = lookupIt->second;

    if (order.side == OrderSide::BUY) {

        std::map<double, std::list<Order>>::iterator level;
        level = bids.find(order.price);

        if (level != bids.end()) {

            std::list<Order>::iterator orderIt;

            for (orderIt = level->second.begin();
                 orderIt != level->second.end();
                 ++orderIt) {

                if (orderIt->id == orderId) {
                    level->second.erase(orderIt);
                    break;
                }
            }

            if (level->second.empty()) {
                bids.erase(level);
            }
        }

    } else {

        std::map<double, std::list<Order>>::iterator level;
        level = asks.find(order.price);

        if (level != asks.end()) {

            std::list<Order>::iterator orderIt;

            for (orderIt = level->second.begin();
                 orderIt != level->second.end();
                 ++orderIt) {

                if (orderIt->id == orderId) {
                    level->second.erase(orderIt);
                    break;
                }
            }

            if (level->second.empty()) {
                asks.erase(level);
            }
        }
    }

    orderLookup.erase(lookupIt);

    return true;
}

bool OrderBook::reduceOrder(long long orderId, int quantity) {

    std::map<long long, Order>::iterator lookupIt = orderLookup.find(orderId);

    if (lookupIt == orderLookup.end()) {
        return false;
    }

    if (quantity >= lookupIt->second.quantity) {
        return cancelOrder(orderId);
    }

    Order order = lookupIt->second;

    orderLookup[orderId].quantity -= quantity;

    if (order.side == OrderSide::BUY) {

        std::map<double, std::list<Order>>::iterator level;
        level = bids.find(order.price);

        if (level != bids.end()) {

            std::list<Order>::iterator orderIt;

            for (orderIt = level->second.begin();
                 orderIt != level->second.end();
                 ++orderIt) {

                if (orderIt->id == orderId) {
                    orderIt->quantity -= quantity;
                    break;
                }
            }
        }

    } else {

        std::map<double, std::list<Order>>::iterator level;
        level = asks.find(order.price);

        if (level != asks.end()) {

            std::list<Order>::iterator orderIt;

            for (orderIt = level->second.begin();
                 orderIt != level->second.end();
                 ++orderIt) {

                if (orderIt->id == orderId) {
                    orderIt->quantity -= quantity;
                    break;
                }
            }
        }
    }

    return true;
}

bool OrderBook::modifyOrder(long long orderId, double newPrice, int newQuantity) {

    Order existingOrder;
    bool found = false;

    for (std::map<double, std::list<Order>>::iterator level = bids.begin();
         level != bids.end();
         ++level) {

        for (std::list<Order>::iterator orderIt = level->second.begin();
             orderIt != level->second.end();
             ++orderIt) {

            if (orderIt->id == orderId) {
                existingOrder = *orderIt;
                found = true;
                break;
            }
        }

        if (found) {
            break;
        }
    }

    if (!found) {
        for (std::map<double, std::list<Order>>::iterator level = asks.begin();
             level != asks.end();
             ++level) {

            for (std::list<Order>::iterator orderIt = level->second.begin();
                 orderIt != level->second.end();
                 ++orderIt) {

                if (orderIt->id == orderId) {
                    existingOrder = *orderIt;
                    found = true;
                    break;
                }
            }

            if (found) {
                break;
            }
        }
    }

    if (!found) {
        return false;
    }

    cancelOrder(orderId);

    existingOrder.price = newPrice;
    existingOrder.quantity = newQuantity;

    addOrder(existingOrder);

    return true;
}

void OrderBook::printBook() const {
    std::cout << "\n===== ORDER BOOK =====\n";

    std::cout << "\nBIDS:\n";

    for (std::map<double, std::list<Order>>::const_reverse_iterator it = bids.rbegin();
         it != bids.rend();
         ++it) {

        std::cout << it->first << " -> ";

        for (std::list<Order>::const_iterator orderIt = it->second.begin();
             orderIt != it->second.end();
             ++orderIt) {

            std::cout << "[ID: " << orderIt->id
                      << ", Qty: " << orderIt->quantity << "] ";
        }

        std::cout << "\n";
    }

    std::cout << "\nASKS:\n";

    for (std::map<double, std::list<Order>>::const_iterator it = asks.begin();
         it != asks.end();
         ++it) {

        std::cout << it->first << " -> ";

        for (std::list<Order>::const_iterator orderIt = it->second.begin();
             orderIt != it->second.end();
             ++orderIt) {

            std::cout << "[ID: " << orderIt->id
                      << ", Qty: " << orderIt->quantity << "] ";
        }

        std::cout << "\n";
    }
}