/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Queue
*/

#include <memory>

#include "IMesagges.hpp"

#ifndef QUEUE_HPP_
#define QUEUE_HPP_

class Queue : public IMesagges {
    public:
        int kitchenId;
        int nbCurrentOrders;

        /* Constructor */
        Queue(int id, int currentOrders);
        ~Queue() override = default;

        /* Pack */
        std::string pack(const IMesagges &messages) const override;

        /* Unpack */
        std::shared_ptr<IMesagges> unpack(const std::string &data) override;

        MessageType getType() const override;
        std::string typeToString(MessageType type) const override;
    protected:
    private:
};

#endif /* !QUEUE_HPP_ */
