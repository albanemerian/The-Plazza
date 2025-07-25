/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Order
*/

#include <iostream>
#include <sstream>
#include <regex>
#include <string>

#include "../APizza.hpp"
#include "IMesagges.hpp"


class Order : public IMesagges {
    public:
        RecipyType type;
        Size size;
        int number;

    /* Constructor */
    Order(RecipyType t, Size s, int n);
    ~Order() override = default;

    /* Pack order Message */
    std::string pack(const IMesagges &order) const override;

    /* Unpack Order */
    std::shared_ptr<IMesagges> unpack(const std::string &data) override;

    MessageType getType() const override;
    std::string typeToString(MessageType type) const override;
};
