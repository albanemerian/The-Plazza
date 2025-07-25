/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Status
*/


#include <memory>

#include "../Ingridient.hpp"
#include "IMesagges.hpp"

#ifndef ASTATUS_HPP_
#define ASTATUS_HPP_

class AStatus : public IMesagges {
    public:
        int kitchenId;
        std::vector<ingStat> status;

        /* Constrcutor */
        AStatus(int id, std::vector<ingStat> status);
        ~AStatus() override = default;

        /* Pack */
        std::string pack(const IMesagges &messages) const override;

        /* Unpack */
        virtual std::shared_ptr<IMesagges> unpack(const std::string &data) override = 0;

        /* Ox to str */
        std::string typeToString(MessageType type) const override;

        /* Virtual function that needs to be override */
        virtual MessageType getType() const override = 0;
    protected:
    private:
};

#endif /* !ASTATUS_HPP_ */
