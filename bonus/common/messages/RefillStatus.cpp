/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** RefillStatus
*/

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <regex>
#include <iomanip>

#include "RefillStatus.hpp"

/* Constructor */
RefillStatus::RefillStatus(int id, std::vector<ingStat> status)
    : AStatus(id, status) {}

/* Get 0x04 type */
MessageType RefillStatus::getType() const {
    return MessageType::Refill;
}

/* Unpack */

std::shared_ptr<IMesagges> RefillStatus::unpack(const std::string &data) {
    RefillStatus status(0, {});
    std::regex pattern("0x04\\|(\\d+)\\|((\\d+:\\d+,?)+);");
    std::smatch match;
    if (std::regex_match(data, match, pattern)) {
        status.kitchenId = std::stoi(match[1].str());
        this->kitchenId = status.kitchenId;
        std::string ingredients = match[2].str();
        std::stringstream ss(ingredients);
        std::string item;
        while (std::getline(ss, item, ',')) {
            size_t pos = item.find(':');
            if (pos != std::string::npos) {
                ingStat ing;
                ing.type = static_cast<IngridientType>
                    (std::stoi(item.substr(0, pos)));
                ing.quantity = std::stoi(item.substr(pos + 1));
                status.status.push_back(ing);
                this->status.push_back(ing);
            }
        }
    }
    return std::make_shared<RefillStatus>(status);
}
