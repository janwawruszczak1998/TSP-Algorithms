//
// Created by Jan on 2020-01-05.
//
#include "Graph.h"
#include "Ant.h"
#ifndef PEA_ACO_H
#define PEA_ACO_H


void ACO(Graph*);
void calculate_route(Ant*, std::vector< std::vector<int> >&, Graph*, std::vector< std::vector<double> >&);
double Phi(int, int, Ant*, Graph*, std::vector< std::vector<double> >&);
int getNextCity(std::vector<double>&);
void updatePheromones(std::vector< std::vector<double> >&, std::vector< std::vector<int> >&, Graph*);
int route_value(std::vector<int>&, Graph*);

#endif //PEA_ACO_H
