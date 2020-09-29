//
// Created by Jan on 2019-10-07.
//
#include "ExactAlgorithms.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <set>


//Dynamik
//
//

void DP(std::unique_ptr<Graph> &g) {

    //preprocessing
    unsigned n = g->get_graph_order();
    unsigned **dp;
    dp = new unsigned *[1 << n];
    for (unsigned i = 0; i < (1 << n); ++i) dp[i] = new unsigned[n];

    for (unsigned i = 0; i < (1 << n); ++i) {
        for (unsigned j = 0; j < n; ++j) {
            dp[i][j] = 1 << 30;    //wypełnienie tablicy tras nieskończonościami
        }
    }


    for (unsigned i = 0; i < n; ++i) {
        dp[(1 << i) |
           1][i] = g->get_adjacency_matrix()[0][i]; //na początku koszt jednokrawędziowej ścieżki Hamiltona z 0 do I to po prostu koszt krawędzi
    }

    //dynamic programming
    for (unsigned bitMask = 0; bitMask < (1 << n); ++bitMask) {                        //dla każdej maski bitowej liczymy trasy
        for (unsigned v = 0; v < n; ++v) {                                             //rozważamy trasy zakończone w wierzchołku V
            if (!(bitMask & (1 << v))) {
                continue;                                                          //jeżeli w ścieżce nie ma w ogóle wierzchołja V, to pomijamy
            }
            for (unsigned j = 0; j < n; ++j) {                                         //patrzymy, do którego wierzchołka J możemy dojść z V
                if (!(bitMask & (1 << j))) {                                     //jeśli wierzchołka J nie ma w trasie
                    dp[bitMask | (1 << j)][j]
                        = std::min(dp[bitMask][v] + g->get_adjacency_matrix()[v][j], dp[bitMask |(1 << j)][j]);    //jeżeli koszt dojścia do V i przejścia do J jest mniejszy od aktualnie najlepszego osiągnięcia J, to zaktualizuj
                }
            }
        }
    }

    //ustalenie resultu
    unsigned result = 1e9;
    for (unsigned v = 0; v < n; ++v) {
        unsigned act = dp[(1 << n) - 1][v] +
                  g->get_adjacency_matrix()[v][0]; //koszt "powrotu" z wierzchołka v do wierzchołka 0
        if (result > act) result = act;
    }

    //zwolnienie pamięci
    for (unsigned i = 0; i < (1 << n); ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    std::cout << "Najtańszy cykl Hamiltona wyznaczona DP: " << result << "\n";
}


void BF(std::unique_ptr<Graph> &g) {
    std::list<unsigned> l{};
    unsigned *best = new unsigned{1 << 30};
    BF_search(g, best, l);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona BF: " << *best << "\n";
    delete best;
}

void BF_search(std::unique_ptr<Graph> &g, unsigned *best, std::list<unsigned> &vertices) {

    if (vertices.size() == g->get_graph_order()) {
        unsigned result = calculate_objective(vertices, g);
        if (*best > result) *best = result;
        return;
    }

    for(unsigned i = 0; i < g->get_graph_order(); ++i){
        bool exist = false;
        for(auto element : vertices){
            if(element == i){
                exist = true;
                break;
            }
        }
        if(!exist){
            vertices.push_back(i);
            BF_search(g, best, vertices);
            vertices.pop_back();
        }
    }

}

//algorytm B&B
//
//preprocessing podziału i ograniczeń
void BB(std::unique_ptr<Graph> &g) {
    std::list<unsigned> l;
    vector<unsigned> v(g->get_graph_order(), 0); //lista na sciezki Hamiltona w grafie
    std::pair<std::list<unsigned>, unsigned> vertices = make_pair(l, 0);  //vector na zachlanna sciezke

    v[0] = 1;
    unsigned greedy_pathVal = greedy_path(g, 0, v, 0); //obliczenie zachlannej sciezki

    //ustalenie najlepszej losowej sciezki
    unsigned random_pathVal = 1e9;
    for (unsigned i = 0; i < g->get_graph_order() * g->get_graph_order(); ++i) {
        random_pathVal = std::min(random_pathVal, random_path(g->get_graph_order(), g));
    }

    auto minimal_edges = get_minimal_edges(g); //tablica najtanszych krawedzi z kazdego wierzcholka
    unsigned lowerBound = calculate_lower_bound(minimal_edges, g->get_graph_order());   //obliczenie dolnego ograniczenia

    //wybranie gornego ograniczenia z najlepszej sciezki losowej i zachlannej
    unsigned upperBound = random_pathVal < greedy_pathVal ? random_pathVal : greedy_pathVal;

    BB_search(g, &lowerBound, &upperBound, vertices, minimal_edges);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona B&B: " << upperBound << "\n";

}


void BB_search(std::unique_ptr<Graph> &g, unsigned *lowerBound, unsigned *upperBound, std::pair<std::list<unsigned>, unsigned> &vertices,
               std::vector<unsigned>& minimal_edges) {
    //nie rozwazamy sciezek gorszych, niz oszacowanie gorne cyklu
    if (vertices.second > *upperBound) return;

    if (vertices.first.size() == g->get_graph_order()) {
        unsigned result = calculate_objective(vertices.first, g);
        if (*upperBound > result) *upperBound = result;
        return;
    }

    for (unsigned i = 0; i < g->get_graph_order(); ++i) {
        bool exist = false;
        for (unsigned j : vertices.first) {
            if (j == i) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            vertices.second += g->get_adjacency_matrix()[vertices.first.back()][i];
            *lowerBound = *lowerBound - minimal_edges[i] + g->get_adjacency_matrix()[vertices.first.back()][i];
            vertices.first.push_back(i);
            if ((*upperBound >= *lowerBound) && (vertices.second <=
                                                 *upperBound))//jezeli przejście do wierzchołka nie przekresla szansy na otrzymanie lepszego cyklu Hamiltona
            {
                BB_search(g, lowerBound, upperBound, vertices, minimal_edges);
            }
            vertices.first.pop_back();
            *lowerBound = *lowerBound + minimal_edges[i] - g->get_adjacency_matrix()[vertices.first.back()][i];
            vertices.second -= g->get_adjacency_matrix()[vertices.first.back()][i];
        } else continue;
    }
}

template<typename Container>
unsigned calculate_objective(Container permutation, std::unique_ptr<Graph> &g) {
    unsigned result = 0, start = permutation.front(), act{};
    while( !permutation.empty() ){
        permutation.pop_front();
        result += g->get_adjacency_matrix()[act][permutation.front()];
        act = permutation.front();
    }
    result += g->get_adjacency_matrix()[act][start];
    return result;

}


unsigned calculate_lower_bound(const std::vector<unsigned>& minimal_edges, const unsigned size) {
    unsigned sum = 0;
    for (unsigned i = 0; i < size; ++i) {
        sum += minimal_edges[i];
    }

    return sum;
}

std::vector<unsigned> get_minimal_edges(std::unique_ptr<Graph> &g) {
    unsigned n = g->get_graph_order();
    std::vector<unsigned> minimal_edges(n);
    for (unsigned i = 0; i < n; ++i) {
        unsigned min = 1e9;
        for (unsigned j = 0; j < n; ++j) {
            if (i != j && min > g->get_adjacency_matrix()[i][j]) min = g->get_adjacency_matrix()[i][j];
        }
        minimal_edges[i] = min;
    }
    return minimal_edges;
}

unsigned random_path(unsigned n, std::unique_ptr<Graph> &g) {

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::list<unsigned> randomPerm;
    std::vector<unsigned> v(n);
    for (unsigned i = 0; i < n; ++i) {
        v[i] = i;
    }
    std::shuffle(v.begin(), v.end(), generator);     //przetasowanie na vectorze
    for (unsigned i = 0; i < n; ++i) {
        randomPerm.push_back(v[i]);
    }
    return calculate_objective(randomPerm, g);

}

unsigned greedy_path(std::unique_ptr<Graph> &g, unsigned row, vector<unsigned> &visited, unsigned result) {
    if (row == g->get_graph_order() - 1) {
        result += g->get_adjacency_matrix()[row][0];
        return result;
    }
    unsigned minValInRow = 1e9;
    unsigned nearestNeighbour = 0;
    for (unsigned i = 0; i < g->get_graph_order(); ++i) {
        if (visited[i]) continue;
        if (minValInRow > g->get_adjacency_matrix()[row][i] && i != row) {
            minValInRow = g->get_adjacency_matrix()[row][i];
            nearestNeighbour = i;
            visited[i] = 1;
        }
    }
    result += g->get_adjacency_matrix()[row][nearestNeighbour];

    return greedy_path(g, ++row, visited, result);

}