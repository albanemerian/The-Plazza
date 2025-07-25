/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Boloss
*/

#include "../../../common/APasta.hpp"

#ifndef BOLOGNESE_HPP_
    #define BOLOGNESE_HPP_

class BologneseClass : public APasta {
    public:
        BologneseClass(int number);
        ~BologneseClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    private:
};

#endif /* !BOLOGNESE_HPP_ */
