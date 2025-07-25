/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** MargaritaCLASS
*/

#include "../../../common/APizza.hpp"

#ifndef MARGARITACLASS_HPP_
#define MARGARITACLASS_HPP_

class MargaritaClass : public APizza {
    public:
        MargaritaClass(int number);
        ~MargaritaClass() override;

        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;


    protected:
    private:
};

#endif /* !MARGARITACLASS_HPP_ */
