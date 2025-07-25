/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Socket
*/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <unistd.h>
#include <memory>
#include "AException.hpp"

class Socket {

    public:
        class SocketException : public AException {
            public:
                SocketException(const std::string &message);
        };

        Socket();
        ~Socket();

        // Server operations
        void createServer(const std::string &sockPath);
        void acceptClient();
        void closeServer();

        // Client operations
        void connectToServer(const std::string &sockPath);
        void closeClient();

        // Common operations
        ssize_t send(const std::string &message);
        std::string receive(size_t size = 1024);
        bool isConnected() const;

        // Operators
        Socket& operator<<(const std::string &message);
        Socket& operator>>(std::string &message);

    private:
        int _serverFd;
        int _clientFd;
        struct sockaddr_un _addr;
        std::string _sockPath;
        bool _isServer;
        bool _isConnected;
};

#endif /* !SOCKET_HPP_ */
