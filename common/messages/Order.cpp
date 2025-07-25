/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Order
*/
#include <memory>
#include <string>
#include <iomanip>

#include "Order.hpp"

/* Constructor */
Order::Order(PizzaType t, Size s, int n)
    : type(t), size(s), number(n) {}

/* Pack */
std::string Order::pack(const IMesagges &order) const {
    const Order &ord = static_cast<const Order&>(order);
    std::ostringstream oss;
    oss << "0x01|" << static_cast<int>(ord.type) << "|"
        << static_cast<int>(ord.size) << "|"
        << ord.number << ";";
    return oss.str();
}

/* Unpack */
std::shared_ptr<IMesagges> Order::unpack(const std::string &data) {
    Order order(Nothing, Zero, 0);
    std::regex pattern("0x01\\|(\\d+)\\|(\\d+)\\|(\\d+);");
    std::smatch match;
    if (std::regex_match(data, match, pattern)) {
        order.type = static_cast<PizzaType>(std::stoi(match[1].str()));
        order.size = static_cast<Size>(std::stoi(match[2].str()));
        order.number = std::stoi(match[3].str());
        this->type = order.type;
        this->size = order.size;
        this->number = order.number;
    }
    return std::make_shared<Order>(order);
}

/* Get Type */
MessageType Order::getType() const {
    return MessageType::Order;
}

std::string Order::typeToString(MessageType type) const {
    std::ostringstream oss;
    oss << "0x"
        << std::uppercase << std::setfill('0') << std::setw(2)
        << std::hex << static_cast<int>(type);
    return oss.str();
}
