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

int BF_search(Graph *, int*, std::list<int> &);
int BB_search(Graph *, int*, int*, std::pair<std::list<int>, int> &, int *);
int calculate_objective(std::list<int>& , Graph *);
int random_path(int, Graph *);
int greedy_path(Graph *, int, vector<int> &, int);
int* get_minimal_edges(Graph *);
int calculate_lower_bound(int *, int);

#endif //PEA_EXACTALGORITHMS_H
