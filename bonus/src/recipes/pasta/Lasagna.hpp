/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Lasagna
*/

#include "../../../common/APasta.hpp"

#ifndef LASAGNA_HPP_
    #define LASAGNA_HPP_

class LasagnaClass : public APasta {
    public:
        LasagnaClass(int number);
        ~LasagnaClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    private:
};

#endif /* !LASAGNA_HPP_ */
