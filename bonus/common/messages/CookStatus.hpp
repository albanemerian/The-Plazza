/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** CookStatus
*/

#include <vector>
#include <memory>

#include "IMesagges.hpp"

#ifndef COOKSTATUS_HPP_
#define COOKSTATUS_HPP_

struct CookStatusData {
    int cookId;
    bool isBusy;
    bool isRestocking;
};

class CookStatus : public IMesagges {
    public:
        int _kitchenId;
        std::vector<CookStatusData> _cooksStatus;

        CookStatus(int kitchenID, const std::vector<CookStatusData> &cooksStatus);
        ~CookStatus() override = default;

        /* Pack */
        std::string pack(const IMesagges &messages) const override;

        /* Unpack */
        std::shared_ptr<IMesagges> unpack(const std::string &data) override;

        MessageType getType() const override;
        std::string typeToString(MessageType type) const override;
    protected:
    private:
};

#endif /* !COOKSTATUS_HPP_ */
