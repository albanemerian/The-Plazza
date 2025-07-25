/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Arrabiata
*/

#include "../../../common/APasta.hpp"

#ifndef ARRABIATA_HPP_
    #define ARRABIATA_HPP_

class ArrabiataClass : public APasta {
    public:
        ArrabiataClass(int number);
        ~ArrabiataClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    protected:
    private:
};

#endif /* !ARRABIATA_HPP_ */
