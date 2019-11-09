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

int BFSearch(Graph *, int*, std::list<int> &);
int BBSearch(Graph *, int*, int*, std::pair<std::list<int>, int> &, int *);
int calculateObjective(std::list<int> , Graph *);
int randomPath(int, Graph *);
int greedyPath(Graph *, int, vector<int> &, int);
int* getMinimalEdges(Graph *);
int calculateLowerBound(int *, int);

#endif //PEA_EXACTALGORITHMS_H
