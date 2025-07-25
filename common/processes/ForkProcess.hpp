/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ForkProcess
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdexcept>
#include <functional>
#include <iostream>

#include "IProcess.hpp"

#ifndef FORKPROCESS_HPP_
#define FORKPROCESS_HPP_

class ForkProcess : public IProcess {
    class ProcessException : public std::runtime_error {
        public:
            explicit ProcessException(const std::string& msg) : std::runtime_error(msg) {}
    };
    public:
        ForkProcess();
        ~ForkProcess() override;

        /* Override Methods */
        pid_t create(const std::function<void()> &childLogic) override;
        int wait() override;
        void close() override;
        pid_t getPid() const override;

    protected:
    private:
        pid_t _pid;
};

#endif /* !FORKPROCESS_HPP_ */
