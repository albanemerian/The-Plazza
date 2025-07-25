/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Plazza
*/

#ifndef PLAZZA_HPP_
#define PLAZZA_HPP_

#include "reception/Reception.hpp"
#include "../common/AException.hpp"
#include "../common/DebugLogger.hpp"

class Plazza {

    class ErrorParsing : public AException {
        public:
            ErrorParsing(const std::string &message);
    };

    public:
        Plazza();
        ~Plazza();

        void parseCmd(char **av, int ac);
        void orderingLoop();

    private:
        int _nbCooks;
        int _timerCooker;
        int _timerRestock;
        bool _debug;
        Reception _reception;
        std::shared_ptr<DebugLogger> _logger;
};

#endif /* !PLAZZA_HPP_ */
