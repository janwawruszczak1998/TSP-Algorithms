//
// Created by Jan on 2020-01-05.
//
#ifndef PEA_ACO_HPP
#define PEA_ACO_HPP

#include "Graph.hpp"
#include "Ant.hpp"


void ACO(std::unique_ptr<Graph> &);

void calculate_route(Ant *, std::vector<std::vector<unsigned> > &, std::unique_ptr<Graph> &, std::vector<std::vector<double> > &);

double Phi(unsigned, unsigned, Ant *, std::unique_ptr<Graph> &, std::vector<std::vector<double> > &);

unsigned getNextCity(std::vector<double> &);

void updatePheromones(std::vector<std::vector<double> > &, std::vector<std::vector<unsigned> > &, std::unique_ptr<Graph> &);

unsigned route_value(std::vector<unsigned> &, std::unique_ptr<Graph> &);

#endif //PEA_ACO_HPP
