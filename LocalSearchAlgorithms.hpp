//
// Created by Jan on 2019-11-25.
//

#ifndef PEA_LOCALSEARCHALGORITHMS_HPP
#define PEA_LOCALSEARCHALGORITHMS_HPP

#include <bits/stdc++.h>
#include "Graph.hpp"

using namespace std;

void TS(std::unique_ptr<Graph> &g);

void SA(std::unique_ptr<Graph> &g);

double get_probability(unsigned, double);

unsigned calculate_objective(std::vector<unsigned> &, std::unique_ptr<Graph> &);

double cooling(double);

void partial_invert(vector<unsigned> &, unsigned, unsigned);


#endif //PEA_LOCALSEARCHALGORITHMS_HPP
