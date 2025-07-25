/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** IExeption
*/

#include <exception>
#include <string>

#ifndef IEXEPTION_HPP_
    #define IEXEPTION_HPP_

class IException : public std::exception {
    public:
        virtual ~IException() noexcept = default;
        const char* what() const noexcept override = 0;
        virtual std::string getType() const noexcept = 0;
        virtual std::string getMessage() const noexcept = 0;
        virtual std::string getFormattedMessage() const noexcept = 0;
};

#endif /* !IEXEPTION_HPP_ */
