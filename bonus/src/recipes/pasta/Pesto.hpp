/*
** EPITECH PROJECT, 2025
** Friteuse
** File description:
** Pesto
*/

#include "../../../common/APasta.hpp"

#ifndef PESTO_HPP_
    #define PESTO_HPP_

class PestoClass : public APasta {
    public:
        PestoClass(int number);
        ~PestoClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    private:
};

#endif /* !PESTO_HPP_ */
