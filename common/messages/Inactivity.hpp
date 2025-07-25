/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Inactivity
*/


#include <memory>

#include "IMesagges.hpp"

#ifndef INACTIVITY_HPP_
#define INACTIVITY_HPP_

class Inactivity : public IMesagges {
    public:
        int id;

        /* Constructor */
        Inactivity(int kitchenId);
        ~Inactivity() override = default;

        /* Pack */
        std::string pack(const IMesagges &messages) const override;

        /* Unpack */
        std::shared_ptr<IMesagges> unpack(const std::string &data) override;

        MessageType getType() const override;
        std::string typeToString(MessageType type) const override;
    protected:
    private:
};

#endif /* !INACTIVITY_HPP_ */
