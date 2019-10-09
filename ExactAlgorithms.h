//
// Created by Jan on 2019-10-07.
//

#ifndef PEA_EXACTALGORITHMS_H
#define PEA_EXACTALGORITHMS_H
#include <bits/stdc++.h>
#include "Graph.h"
using namespace std;

void DP(Graph *g);
void BF(Graph *g);
void BB(Graph *g);

int treeSearch(Graph *g, int*, std::list<int>);
int calculateObjective(std::list<int> , Graph *g);

#endif //PEA_EXACTALGORITHMS_H
