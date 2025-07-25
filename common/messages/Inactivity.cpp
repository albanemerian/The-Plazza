/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Inactivity
*/

#include <string>
#include <regex>
#include <memory>
#include <iomanip>

#include "Inactivity.hpp"


/* Constructor */

Inactivity::Inactivity(int kitchenId)
    : id(kitchenId) {}

/* Pack */
std::string Inactivity::pack(const IMesagges &messages) const {
    const Inactivity &inactivity =
        static_cast<const Inactivity&>(messages);
    return "0x03|" + std::to_string(inactivity.id) + ";";
}

/* Unpack */
std::shared_ptr<IMesagges> Inactivity::unpack(const std::string &data) {
    Inactivity inactivity(0);
    std::regex pattern("0x03\\|(\\d+);");
    std::smatch match;
    if (std::regex_match(data, match, pattern)) {
        inactivity.id = std::stoi(match[1].str());
        this->id = inactivity.id;
    }
    return std::make_shared<Inactivity>(inactivity);
}


/* Get type */

MessageType Inactivity::getType() const {
    return MessageType::Inactivity;
}


std::string Inactivity::typeToString(MessageType type) const {
    std::ostringstream oss;
    oss << "0x"
        << std::uppercase << std::setfill('0') << std::setw(2)
        << std::hex << static_cast<int>(type);
    return oss.str();
}
