/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** IProcess
*/

#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <functional>

#ifndef IPROCESS_HPP_
#define IPROCESS_HPP_

class IProcess {
    public:
        virtual pid_t create(const std::function<void()> &childLogic) = 0;
        virtual int wait() = 0;
        virtual void close() = 0;
        virtual pid_t getPid() const = 0;
        virtual ~IProcess() = default;
    protected:
    private:
};

#endif /* !IPROCESS_HPP_ */
