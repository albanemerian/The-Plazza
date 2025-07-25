/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Refill
*/

#include "AStatus.hpp"

#ifndef REFILLSTATUS_HPP_
#define REFILLSTATUS_HPP_

class RefillStatus : public AStatus {
    public:
        RefillStatus(int id, std::vector<ingStat> status);
        MessageType getType() const override;
        std::shared_ptr<IMesagges> unpack(const std::string &data) override;

    protected:
    private:
};

#endif /* !REFILLSTATUS_HPP_ */
