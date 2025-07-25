/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Paffo
*/

#include "../../../common/APasta.hpp"

#ifndef PAFFO_HPP_
    #define PAFFO_HPP_

class PaffoClass : public APasta {
    public:
        PaffoClass(int number);
        ~PaffoClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    private:
};

#endif /* !PAFFO_HPP_ */
