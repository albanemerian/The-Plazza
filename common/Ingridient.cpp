/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Ingridient
*/

#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <memory>

#include "Ingridient.hpp"

Ingridient::Ingridient() {
    this->_dough = 5;
    this->_tomato = 5;
    this->_cheese = 5;
    this->_ham = 5;
    this->_mushroom = 5;
    this->_steak = 5;
    this->_eggplant = 5;
    this->_goatCheese = 5;
    this->_chefLove = 5;
    this->_egg = 5;
    this->_bacon = 5;
    this->_basil = 5;
    this->_pepper = 5;
    this->_ingridient.push_back({DOUGH, this->_dough});
    this->_ingridient.push_back({TOMATO, this->_tomato});
    this->_ingridient.push_back({CHEESE, this->_cheese});
    this->_ingridient.push_back({HAM, this->_ham});
    this->_ingridient.push_back({MUSHROOM, this->_mushroom});
    this->_ingridient.push_back({STEAK, this->_steak});
    this->_ingridient.push_back({EGGPLANT, this->_eggplant});
    this->_ingridient.push_back({GOAT_CHEESE, this->_goatCheese});
    this->_ingridient.push_back({CHEF_LOVE, this->_chefLove});
    this->_ingridient.push_back({EGG, this->_egg});
    this->_ingridient.push_back({BACON, this->_bacon});
    this->_ingridient.push_back({BASIL, this->_basil});
    this->_ingridient.push_back({PEPPER, this->_pepper});
}

/* Method */
std::vector<ingStat> Ingridient::fridgeStatus() {
    std::vector<ingStat> status;
    status.push_back({DOUGH, this->_dough});
    status.push_back({TOMATO, this->_tomato});
    status.push_back({CHEESE, this->_cheese});
    status.push_back({HAM, this->_ham});
    status.push_back({MUSHROOM, this->_mushroom});
    status.push_back({STEAK, this->_steak});
    status.push_back({EGGPLANT, this->_eggplant});
    status.push_back({GOAT_CHEESE, this->_goatCheese});
    status.push_back({CHEF_LOVE, this->_chefLove});
    status.push_back({EGG, this->_egg});
    status.push_back({BACON, this->_bacon});
    status.push_back({BASIL, this->_basil});
    status.push_back({PEPPER, this->_pepper});
    return status;
}

std::string Ingridient::packIngredients() const {
    std::ostringstream packed;

    if (_dough > 0) packed << DOUGH << ":" << _dough << "|";
    if (_tomato > 0) packed << TOMATO << ":" << _tomato << "|";
    if (_cheese > 0) packed << CHEESE << ":" << _cheese << "|";
    if (_ham > 0) packed << HAM << ":" << _ham << "|";
    if (_mushroom > 0) packed << MUSHROOM << ":" << _mushroom << "|";
    if (_steak > 0) packed << STEAK << ":" << _steak << "|";
    if (_eggplant > 0) packed << EGGPLANT << ":" << _eggplant << "|";
    if (_goatCheese > 0) packed << GOAT_CHEESE << ":" << _goatCheese << "|";
    if (_chefLove > 0) packed << CHEF_LOVE << ":" << _chefLove << "|";
    if (_egg > 0) packed << EGG << ":" << _egg << "|";
    if (_bacon > 0) packed << BACON << ":" << _bacon << "|";
    if (_basil > 0) packed << BASIL << ":" << _basil << "|";
    if (_pepper > 0) packed << PEPPER << ":" << _pepper << "|";

    std::string result = packed.str();
    if (!result.empty() && result.back() == '|') {
        result.pop_back();
    }
    return result;
}

std::map<IngridientType, int> Ingridient::unpackIngredients(const
    std::string& packedData) {
    std::map<IngridientType, int> result;
    std::istringstream stream(packedData);
    std::string token;

    while (std::getline(stream, token, '|')) {
        size_t colonPos = token.find(':');
        if (colonPos != std::string::npos) {
            int type = std::stoi(token.substr(0, colonPos));
            int qty = std::stoi(token.substr(colonPos + 1));
            result[static_cast<IngridientType>(type)] = qty;
        }
    }
    return result;
}

/* Getter */

int Ingridient::getDough() const {
    return this->_dough;
}

int Ingridient::getTomato() const {
    return this->_tomato;
}

int Ingridient::getCheese() const {
    return this->_cheese;
}

int Ingridient::getHam() const {
    return this->_ham;
}

int Ingridient::getMushroom() const {
    return this->_mushroom;
}

int Ingridient::getSteak() const {
    return this->_steak;
}

int Ingridient::getEggplant() const {
    return this->_eggplant;
}

int Ingridient::getGoatCheese() const {
    return this->_goatCheese;
}

int Ingridient::getChefLove() const {
    return this->_chefLove;
}

int Ingridient::getEgg() const {
    return this->_egg;
}

int Ingridient::getBacon() const {
    return this->_bacon;
}

int Ingridient::getBasil() const {
    return this->_basil;
}

int Ingridient::getPepper() const {
    return this->_pepper;
}

/* Setter */

void Ingridient::setDough(int dough) {
    this->_dough = dough;
}

void Ingridient::setTomato(int tomato) {
    this->_tomato = tomato;
}

void Ingridient::setCheese(int cheese) {
    this->_cheese = cheese;
}

void Ingridient::setHam(int ham) {
    this->_ham = ham;
}

void Ingridient::setMushroom(int mushroom) {
    this->_mushroom = mushroom;
}

void Ingridient::setSteak(int steak) {
    this->_steak = steak;
}

void Ingridient::setEggplant(int eggplant) {
    this->_eggplant = eggplant;
}

void Ingridient::setGoatCheese(int goatCheese) {
    this->_goatCheese = goatCheese;
}

void Ingridient::setChefLove(int chefLove) {
    this->_chefLove = chefLove;
}

void Ingridient::setEgg(int egg) {
    this->_egg = egg;
}

void Ingridient::setBacon(int bacon) {
    this->_bacon = bacon;
}

void Ingridient::setBasil(int basil) {
    this->_basil = basil;
}

void Ingridient::setPepper(int pepper) {
    this->_pepper = pepper;
}


/* Operator */

std::shared_ptr<Ingridient> Ingridient::operator=(const
    std::vector<ingStat> &ingStat) {
    std::shared_ptr<Ingridient> newIngridient = std::make_shared<Ingridient>();
    for (const auto &stat : ingStat) {
        switch (stat.type) {
            case DOUGH:
                newIngridient->setDough(stat.quantity);
                break;
            case TOMATO:
                newIngridient->setTomato(stat.quantity);
                break;
            case CHEESE:
                newIngridient->setCheese(stat.quantity);
                break;
            case HAM:
                newIngridient->setHam(stat.quantity);
                break;
            case MUSHROOM:
                newIngridient->setMushroom(stat.quantity);
                break;
            case STEAK:
                newIngridient->setSteak(stat.quantity);
                break;
            case EGGPLANT:
                newIngridient->setEggplant(stat.quantity);
                break;
            case GOAT_CHEESE:
                newIngridient->setGoatCheese(stat.quantity);
                break;
            case CHEF_LOVE:
                newIngridient->setChefLove(stat.quantity);
                break;
            case EGG:
                newIngridient->setEgg(stat.quantity);
                break;
            case BACON:
                newIngridient->setBacon(stat.quantity);
                break;
            case BASIL:
                newIngridient->setBasil(stat.quantity);
                break;
            case PEPPER:
                newIngridient->setPepper(stat.quantity);
                break;
        }
    }
    return newIngridient;
}
