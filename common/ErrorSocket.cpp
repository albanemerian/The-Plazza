/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ErrorSocket
*/

#include <string>


#include "Socket.hpp"

Socket::SocketException::SocketException(const std::string &message)
    : AException("Socket", message) {
}
