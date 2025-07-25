/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Queue
*/

#include <string>
#include <regex>
#include <memory>
#include <iomanip>


#include "Queue.hpp"


/* Constructor */

Queue::Queue(int id, int currentOrders)
    : kitchenId(id), nbCurrentOrders(currentOrders) {}

/* Pack */

std::string Queue::pack(const IMesagges &messages) const {
    const Queue &queue = static_cast<const Queue&>(messages);
    return "0x05|" + std::to_string(queue.kitchenId) + "|" +
        std::to_string(queue.nbCurrentOrders) + ";";
}

/* Unpack */
std::shared_ptr<IMesagges> Queue::unpack(const std::string &data)  {
    Queue queue(0, 0);
    std::regex pattern("0x05\\|(\\d+)\\|(\\d+);");
    std::smatch match;
    if (std::regex_match(data, match, pattern)) {
        queue.kitchenId = std::stoi(match[1].str());
        queue.nbCurrentOrders = std::stoi(match[2].str());
        this->kitchenId = queue.kitchenId;
        this->nbCurrentOrders = queue.nbCurrentOrders;
    }
    return std::make_shared<Queue>(queue);
}


/* Message type */
MessageType Queue::getType() const {
    return MessageType::Queue;
}

std::string Queue::typeToString(MessageType type) const {
    std::ostringstream oss;
    oss << "0x"
        << std::uppercase << std::setfill('0') << std::setw(2)
        << std::hex << static_cast<int>(type);
    return oss.str();
}
