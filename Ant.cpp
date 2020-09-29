//
// Created by Jan on 2020-01-06.
//

#include "Ant.hpp"

Ant::Ant(unsigned number_, unsigned n)
: number(number_){
    tabu.resize(n, false);
}

unsigned Ant::get_number() {
    return number;
}

std::vector<bool>& Ant::get_tabu(){
    return tabu;
}