//
// Created by Jan on 2019-11-25.
//

#ifndef PEA_LOCALSEARCHALGORITHMS_H
#define PEA_LOCALSEARCHALGORITHMS_H

#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

void TS(Graph *g);
void SA(Graph *g);

double get_probability(int ,double);
int random_path(std::vector<int>&, Graph* );
int calculate_objective(std::vector<int>&, Graph *);
double cooling(double, int);


#endif //PEA_LOCALSEARCHALGORITHMS_H
