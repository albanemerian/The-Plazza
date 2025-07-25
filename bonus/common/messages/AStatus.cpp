/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** AStatus
*/

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <regex>
#include <iomanip>


#include "AStatus.hpp"

/* Constructor */

AStatus::AStatus(int id, std::vector<ingStat> status)
    : kitchenId(id), status(status) {}


/* Pack */
std::string AStatus::pack(const IMesagges &messages) const {
    const AStatus &stat = static_cast<const AStatus&>(messages);
    std::ostringstream oss;
    // oss << "0x02|";
    std::string type = typeToString(getType());
    oss << type << "|";
    oss << stat.kitchenId << "|";
    for (const auto &ing : stat.status) {
        oss << static_cast<int>(ing.type) << ":" << ing.quantity << ",";
    }
    std::string packed = oss.str();
    packed.pop_back();
    packed += ";";
    return packed;
}


std::string AStatus::typeToString(MessageType type) const {
    std::ostringstream oss;
    oss << "0x"
        << std::uppercase << std::setfill('0') << std::setw(2)
        << std::hex << static_cast<int>(type);
    return oss.str();
}
