/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Socket
*/

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#include <stdexcept>
#include <iostream>
#include <string>

#include "Socket.hpp"


Socket::Socket()
    : _serverFd(-1), _clientFd(-1), _isServer(false),
    _isConnected(false) {
    memset(&_addr, 0, sizeof(struct sockaddr_un));
}

Socket::~Socket() {
    if (_isServer)
        closeServer();
    else
        closeClient();
}

void Socket::createServer(const std::string &sockPath) {
    _sockPath = sockPath;
    _isServer = true;

    unlink(_sockPath.c_str());

    _serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw SocketException("Failed to create socket");

    _addr.sun_family = AF_UNIX;
    strncpy(_addr.sun_path, _sockPath.c_str(), sizeof(_addr.sun_path) - 1);

    if (bind(_serverFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        close(_serverFd);
        throw SocketException("Failed to bind socket to " + _sockPath);
    }

    if (listen(_serverFd, 5) < 0) {
        close(_serverFd);
        unlink(_sockPath.c_str());
        throw SocketException("Failed to listen on socket");
    }
}

void Socket::acceptClient() {
    if (!_isServer)
        throw SocketException("Not a server socket");

    _clientFd = accept(_serverFd, nullptr, nullptr);
    if (_clientFd < 0)
        throw SocketException("Failed to accept client connection");

    _isConnected = true;
}

void Socket::connectToServer(const std::string &sockPath) {
    _sockPath = sockPath;
    _isServer = false;

    // Create socket
    _clientFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_clientFd < 0)
        throw SocketException("Failed to create client socket");

    // Prepare address
    _addr.sun_family = AF_UNIX;
    strncpy(_addr.sun_path, _sockPath.c_str(), sizeof(_addr.sun_path) - 1);

    // Connect
    if (connect(_clientFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        close(_clientFd);
        throw SocketException("Failed to connect to server at " + _sockPath);
    }
    _isConnected = true;
}

void Socket::closeServer() {
    if (_clientFd >= 0) {
        close(_clientFd);
        _clientFd = -1;
    }
    if (_serverFd >= 0) {
        close(_serverFd);
        _serverFd = -1;
    }
    unlink(_sockPath.c_str());
    _isConnected = false;
}

void Socket::closeClient() {
    if (_clientFd >= 0) {
        close(_clientFd);
        _clientFd = -1;
    }
    _isConnected = false;
}

ssize_t Socket::send(const std::string &message) {
    int fd = _isServer ? _clientFd : _clientFd;
    if (fd < 0)
        throw SocketException("Socket not connected");

    ssize_t bytesSent = write(fd, message.c_str(), message.size());
    if (bytesSent < 0)
        throw SocketException("Failed to send data");

    return bytesSent;
}


std::string Socket::receive(size_t kBufferSize) {
    int fd = _isServer ? _clientFd : _clientFd;
    if (fd < 0)
        throw SocketException("Socket not connected");

    struct pollfd fds[1];
    fds[0].fd = fd;
    fds[0].events = POLLIN;

    int poll_ret = poll(fds, 1, 2500);

    if (poll_ret < 0) {
        return "";
    } else if (poll_ret == 0) {
        return "";
    }

    char buffer[kBufferSize + 1];
    memset(buffer, 0, kBufferSize + 1);

    ssize_t bytesRead = read(fd, buffer, kBufferSize);
    if (bytesRead < 0)
        return "";
    if (bytesRead == 0) {
        _isConnected = false;
        return "";
    }
    return std::string(buffer, bytesRead);
}

bool Socket::isConnected() const {
    return _isConnected;
}

Socket& Socket::operator<<(const std::string &message) {
    send(message);
    return *this;
}

Socket& Socket::operator>>(std::string &message) {
    message = receive();
    return *this;
}

