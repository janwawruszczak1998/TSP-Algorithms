//
// Created by Jan on 2020-01-06.
//

#include "Ant.h"

Ant::Ant(int number, int n){
    this->number = number;
    tabu.resize(n, false);
}
Ant::~Ant(){
    tabu.clear();
}

int Ant::get_number() {
    return number;
}

std::vector<bool>& Ant::get_tabu(){
    return tabu;
}