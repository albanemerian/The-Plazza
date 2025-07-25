/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** AExeption
*/

#ifndef AEXEPTION_HPP_
    #define AEXEPTION_HPP_

#include "IException.hpp"
#include <string>

class AException : public IException {
    public:
        AException(const std::string& type, const std::string& message)
        : _message(message), _type(type) {}
        virtual ~AException() noexcept = default;

        const char* what() const noexcept override {
            return getFormattedMessage().c_str();
        }

        std::string getType() const noexcept override {
            return _type;
        }

        std::string getMessage() const noexcept override {
            return _message;
        }

        std::string getFormattedMessage() const noexcept override {
            return "\033[1;31m[" + _type + "]\033[0m " + _message;
        }

    private:
        std::string _message;
        std::string _type;
};

#endif /* !AEXEPTION_HPP_ */
