/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ReginaClass
*/


#include "../../../common/APizza.hpp"

#ifndef REGINACLASS_HPP_
#define REGINACLASS_HPP_

class ReginaClass : public APizza {
    public:
        ReginaClass(int number);
        ~ReginaClass() override;

        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;
    protected:
    private:
};

#endif /* !REGINACLASS_HPP_ */
