//
// Created by Jan on 2019-10-07.
//

#ifndef PEA_EXACT_ALGORITHMS_H
#define PEA_EXACT_ALGORITHMS_H

#include <vector>
#include <list>
#include <set>
#include <memory>

#include "Graph.hpp"

using namespace std;

void DP(std::unique_ptr <Graph> &g);

void BF(std::unique_ptr <Graph> &g);

void BB(std::unique_ptr <Graph> &g);

void BF_search(std::unique_ptr <Graph> &, unsigned *, std::list<unsigned> &);

void BB_search(std::unique_ptr <Graph> &, unsigned *, unsigned *, std::pair<std::list<unsigned>, unsigned> &,
               std::vector<unsigned> &);

template<typename Container>
unsigned calculate_objective(Container, std::unique_ptr <Graph> &);

unsigned random_path(unsigned, std::unique_ptr <Graph> &);

unsigned greedy_path(std::unique_ptr <Graph> &, unsigned, vector<unsigned> &, unsigned);

std::vector<unsigned> get_minimal_edges(std::unique_ptr <Graph> &);

unsigned calculate_lower_bound(const std::vector<unsigned>&, unsigned);

#endif //PEA_EXACT_ALGORITHMS_H
