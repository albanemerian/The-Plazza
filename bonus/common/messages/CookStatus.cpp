/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** CookStatus
*/

#include <string>
#include <regex>
#include <vector>
#include <memory>
#include <iomanip>

#include "CookStatus.hpp"

/* Constructor */
CookStatus::CookStatus(int kitchenID, const std::vector<CookStatusData>
    &cooksStatus)
    : _kitchenId(kitchenID), _cooksStatus(cooksStatus) {}


/* Pack */
std::string CookStatus::pack(const IMesagges &messages) const {
    const CookStatus &cookStatus = static_cast<const
    CookStatus&>(messages);
    std::string result = "0x06|" + std::to_string(cookStatus._kitchenId)
    + "|";

    for (const auto& cook : cookStatus._cooksStatus) {
        result += std::to_string(cook.cookId) + ":" +
                  (cook.isBusy ? "1" : "0") + ":" +
                  (cook.isRestocking ? "1" : "0") + "|";
    }
    result += ";";
    return result;
}

/* Unpack */
std::shared_ptr<IMesagges> CookStatus::unpack(const std::string &data) {
    std::vector<CookStatusData> cooksStatus;
    int kitchenId = 0;

    std::regex pattern("0x06\\|(\\d+)\\|(.*)");
    std::smatch match;

    if (std::regex_match(data, match, pattern)) {
        kitchenId = std::stoi(match[1].str());
        std::string cooksData = match[2].str();

        if (!cooksData.empty() && cooksData.back() == ';') {
            cooksData.pop_back();
        }

        std::regex cookPattern("(\\d+):(\\d):(\\d)");
        std::sregex_iterator iter(cooksData.begin(), cooksData.end(),
            cookPattern);
        std::sregex_iterator end;

        for (; iter != end; ++iter) {
            std::smatch cookMatch = *iter;
            CookStatusData cook;
            cook.cookId = std::stoi(cookMatch[1].str());
            cook.isBusy = (cookMatch[2].str() == "1");
            cook.isRestocking = (cookMatch[3].str() == "1");
            cooksStatus.push_back(cook);
        }
    }
    return std::make_shared<CookStatus>(kitchenId, cooksStatus);
}


/* Message type */
MessageType CookStatus::getType() const {
    return MessageType::CookStatus;
}

std::string CookStatus::typeToString(MessageType type) const {
    std::ostringstream oss;
    oss << "0x"
        << std::uppercase << std::setfill('0') << std::setw(2)
        << std::hex << static_cast<int>(type);
    return oss.str();
}

