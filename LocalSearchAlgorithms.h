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

double getProbability(int ,double);
int randomPath(std::vector<int>&, Graph* );
int greedyyPath(Graph *, int, vector<int> &, int);
int calculateObjective(std::vector<int>&, Graph *);
double cooling(double, int);


#endif //PEA_LOCALSEARCHALGORITHMS_H
