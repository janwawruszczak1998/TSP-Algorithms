//
// Created by Jan on 2019-10-07.
//

#ifndef PEA_EXACTALGORITHMS_H
#define PEA_EXACTALGORITHMS_H

#include <vector>
#include <list>
#include <set>
#include <memory>

#include "Graph.hpp"

using namespace std;

void DP(std::unique_ptr<Graph> &g);

void BF(std::unique_ptr<Graph> &g);

void BB(std::unique_ptr<Graph> &g);

void BF_search(std::unique_ptr<Graph> &, int *, std::list<int> &);

int BB_search(std::unique_ptr<Graph> &, int *, int *, std::pair<std::list<int>, int> &, int *);

template<typename Container>
int calculate_objective(Container, std::unique_ptr<Graph> &);

int random_path(int, std::unique_ptr<Graph> &);

int greedy_path(std::unique_ptr<Graph> &, int, vector<int> &, int);

int *get_minimal_edges(std::unique_ptr<Graph> &);

int calculate_lower_bound(const int *, int);

#endif //PEA_EXACTALGORITHMS_H
