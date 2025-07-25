/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** DoneStatus
*/

#include "AStatus.hpp"

#ifndef DONESTATUS_HPP_
#define DONESTATUS_HPP_

class DoneStatus : public AStatus {
    public:
        DoneStatus(int id, std::vector<ingStat> status);
        MessageType getType() const override;
        std::shared_ptr<IMesagges> unpack(const std::string &data) override;

    protected:
    private:
};

#endif /* !DONESTATUS_HPP_ */
