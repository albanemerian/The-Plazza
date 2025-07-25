/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** IMesagges
*/

#include <string>
#include <memory>


#ifndef IMESAGGES_HPP_
#define IMESAGGES_HPP_


enum class MessageType : uint8_t {
    Order = 0x01,
    Status = 0x02,
    Inactivity = 0x03,
    Refill = 0x04,
    Queue = 0x05,
    CookStatus = 0x06,
};


class IMesagges {
    public:
        virtual ~IMesagges() = default;
        virtual std::string pack(const IMesagges &messages) const = 0;
        virtual std::shared_ptr<IMesagges> unpack(const std::string &data) = 0;
        virtual MessageType getType() const = 0;
        virtual std::string typeToString(MessageType type) const = 0;

    protected:
    private:
};

#endif /* !IMESAGGES_HPP_ */
