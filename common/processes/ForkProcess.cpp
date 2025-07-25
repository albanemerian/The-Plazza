/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ForkProcess
*/

#include <memory>
#include <string>

#include "ForkProcess.hpp"

ForkProcess::ForkProcess() {
    _pid = -1;
}

ForkProcess::~ForkProcess() {
    close();
}

/* Methods */
pid_t ForkProcess::create(const std::function<void()> &childLogic) {
    pid_t pid = fork();
    if (pid < 0) {
        throw ProcessException("Fork failed");
    }
    if (pid == 0) {
        childLogic();
        _exit(0);
    }
    _pid = pid;
    return pid;
}

int ForkProcess::wait() {
    int status = 0;
    if (_pid > 0)
        waitpid(_pid, &status, 0);
    return status;
}


void ForkProcess::close() {
    if (_pid > 0) {
        kill(_pid, SIGKILL);
        _pid = -1;
    }
}

/* Getter */
pid_t ForkProcess::getPid() const {
    return _pid;
}

extern "C" {
    std::shared_ptr<IProcess> createProcess() {
        return std::make_shared<ForkProcess>();
    }
    std::string getType() {
        return "ForkProcess";
    }
}
